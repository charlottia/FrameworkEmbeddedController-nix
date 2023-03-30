#!/usr/bin/env python3

# Copyright 2022 The ChromiumOS Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import argparse
import dataclasses
import distutils.spawn
import os
import pathlib
import signal
import socket
import subprocess
import sys
import time


"""
Flashes and debugs the EC through openocd
"""

EC_BASE = pathlib.Path(__file__).parent.parent

# GDB variant to use if the board specific one is not found
FALLBACK_GDB_VARIANT = "gdb-multiarch"


@dataclasses.dataclass
class BoardInfo:
    gdb_variant: str
    num_breakpoints: int
    num_watchpoints: int


# Debuggers for each board, OpenOCD currently only supports GDB
boards = {
    "rex": BoardInfo("arm-none-eabi-gdb", 6, 4),
    "skyrim": BoardInfo("arm-none-eabi-gdb", 6, 4),
}


def create_openocd_args(interface, board):
    if not board in boards:
        raise RuntimeError(f"Unsupported board {board}")

    args = [
        "openocd",
        "-f",
        f"interface/{interface}.cfg",
        "-c",
        f"add_script_search_dir {EC_BASE}/util/openocd",
        "-f",
        f"board/{board}.cfg",
    ]

    return args


def create_gdb_args(board, port, executable, attach):
    if not board in boards:
        raise RuntimeError(f"Unsupported board {board}")

    board_info = boards[board]

    if distutils.spawn.find_executable(board_info.gdb_variant):
        gdb_path = board_info.gdb_variant
    elif distutils.spawn.find_executable(FALLBACK_GDB_VARIANT):
        print(
            f"GDB executable {board_info.gdb_variant} not found, "
            f"using {FALLBACK_GDB_VARIANT} instead"
        )
        gdb_path = FALLBACK_GDB_VARIANT
    else:
        raise RuntimeError("No GDB executable found in the system")

    args = [
        gdb_path,
        executable,
        # GDB can't autodetect these according to OpenOCD
        "-ex",
        f"set remote hardware-breakpoint-limit {board_info.num_breakpoints}",
        "-ex",
        f"set remote hardware-watchpoint-limit {board_info.num_watchpoints}",
        # Connect to OpenOCD
        "-ex",
        f"target extended-remote localhost:{port}",
    ]

    if not attach:
        args.extend(["-ex", "load"])

    return args


def flash(interface, board, image, verify):
    print(f"Flashing image {image}")
    # Run OpenOCD and pipe its output to stdout
    # OpenOCD will shutdown after the flashing is completed
    args = create_openocd_args(interface, board)
    args += ["-c", f'init; flash_target "{image}" {int(verify)}; shutdown']

    subprocess.run(args, stdout=sys.stdout, stderr=subprocess.STDOUT)


def debug(interface, board, port, executable, attach):
    # Start OpenOCD in the background
    openocd_args = create_openocd_args(interface, board)
    openocd_args += ["-c", f"gdb_port {port}"]

    openocd = subprocess.Popen(
        openocd_args,
        encoding="utf-8",
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        preexec_fn=os.setsid,
    )

    # Wait for OpenOCD to start, it'll open a port for GDB connections
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connected = False
    for i in range(0, 10):
        print("Waiting for OpenOCD to start...")
        connected = sock.connect_ex(("localhost", port))
        if connected:
            break

        time.sleep(1000)

    if not connected:
        print(f"Failed to connect to OpenOCD on port {port}")
        return

    sock.close()

    old_sigint = signal.signal(signal.SIGINT, signal.SIG_IGN)

    # Start GDB
    gdb_args = create_gdb_args(board, port, executable, attach)
    gdb = subprocess.Popen(
        gdb_args, stdout=sys.stdout, stderr=subprocess.STDOUT, stdin=sys.stdin
    )

    openocd_out = ""
    while gdb.poll() == None and openocd.poll() == None:
        (output, _) = openocd.communicate()
        openocd_out += output

    signal.signal(signal.SIGINT, old_sigint)

    # Wait for OpenOCD to shutdown
    print("Waiting for OpenOCD to finish...")
    if openocd.poll() == None:
        try:
            # Read the last bit of stdout
            (output, _) = openocd.communicate(timeout=3)
            openocd_out += output
        except subprocess.TimeoutExpired:
            # OpenOCD didn't shutdown, kill it
            openocd.kill()

    if openocd.returncode != 0:
        print("OpenOCD failed to shutdown cleanly: ")
    print(openocd_out)


def debug_external(board, port, executable, attach):
    """Run GDB against an external gdbserver."""
    gdb_args = create_gdb_args(board, port, executable, attach)

    old_sigint = signal.signal(signal.SIGINT, signal.SIG_IGN)

    subprocess.run(
        gdb_args,
        stdout=sys.stdout,
        stderr=subprocess.STDOUT,
        stdin=sys.stdin,
        check=True,
    )

    signal.signal(signal.SIGINT, old_sigint)


def get_flash_file(board):
    return (
        EC_BASE / "build" / "zephyr" / board / "output" / "ec.bin"
    ).resolve()


def get_executable_file(board):
    return (
        EC_BASE / "build" / "zephyr" / board / "output" / "zephyr.ro.elf"
    ).resolve()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--board",
        "-b",
        choices=boards.keys(),
        required=True,
    )

    parser.add_argument(
        "--interface",
        "-i",
        default="jlink",
        help="The JTAG interface to use",
    )

    parser.add_argument(
        "--file",
        "-f",
        type=pathlib.Path,
        default=None,
        help="The file to use, see each sub-command for what the file is used for",
    )

    sub_parsers = parser.add_subparsers(help="sub-command -h for specific help")
    flash_parser = sub_parsers.add_parser(
        "flash",
        help="Flashes an image to the target EC, \
        FILE selects the image to flash, defaults to the zephyr image",
    )
    flash_parser.set_defaults(command="flash")
    flash_parser.add_argument(
        "--verify",
        "-v",
        default=True,
        help="Verify flash after writing image, defaults to true",
    )

    debug_parser = sub_parsers.add_parser(
        "debug",
        help="Debugs the target EC through GDB, \
        FILE selects the executable to load debug info from, defaults to using the zephyr RO executable",
    )
    debug_parser.set_defaults(command="debug")
    debug_parser.add_argument(
        "--port",
        "-p",
        help="The port for GDB to connect to",
        type=int,
        default=3333,
    )
    debug_parser.add_argument(
        "--external-gdbserver",
        "-x",
        help="Do not run openocd, use an already running external gdb server",
        action="store_true",
    )
    debug_parser.add_argument(
        "--attach",
        "-a",
        help="Do not load the binary after starting gdb, attach to the running instance instead",
        action="store_true",
    )

    args = parser.parse_args()
    # Get the image path if we were given one
    target_file = None
    if args.file != None:
        target_file = args.file.resolve()

    if args.command == "flash":
        image_file = (
            get_flash_file(args.board) if target_file == None else target_file
        )
        flash(args.interface, args.board, image_file, args.verify)
    elif args.command == "debug":
        executable_file = (
            get_executable_file(args.board)
            if target_file == None
            else target_file
        )
        if args.external_gdbserver:
            debug_external(
                args.board,
                args.port,
                executable_file,
                args.attach,
            )
        else:
            debug(
                args.interface,
                args.board,
                args.port,
                executable_file,
                args.attach,
            )
    else:
        parser.print_usage()


if __name__ == "__main__":
    main()
