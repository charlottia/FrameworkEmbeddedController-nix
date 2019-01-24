/* Copyright 2018 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Arcada ISH board configuration */

#ifndef __CROS_EC_BOARD_H
#define __CROS_EC_BOARD_H

/*
 * Allow dangerous commands.
 * TODO: Remove this config before production.
 */
#define CONFIG_SYSTEM_UNLOCKED

/*
 * By default, enable all console messages except HC, ACPI and event
 * The sensor stack is generating a lot of activity.
 */
#undef CONFIG_HOSTCMD_DEBUG_MODE
#define CONFIG_HOSTCMD_DEBUG_MODE HCDEBUG_OFF

/* ISH specific*/
#undef  CONFIG_DEBUG_ASSERT
#define CONFIG_CLOCK_CRYSTAL
#define CONFIG_ISH_UART_0
/* EC */
#define CONFIG_FLASH_SIZE 0x80000
#define CONFIG_FPU
#define CONFIG_I2C
#define CONFIG_I2C_MASTER

/* I2C ports */
#define I2C_PORT_SENSOR ISH_I2C0
#define CONFIG_CMD_I2C_XFER

/* Undefine features */
#undef CONFIG_CMD_HASH
#undef CONFIG_CMD_I2C_SCAN
#undef CONFIG_CMD_KEYBOARD
#undef CONFIG_CMD_POWER_AP
#undef CONFIG_CMD_POWERINDEBUG
#undef CONFIG_CMD_SHMEM
#undef CONFIG_CMD_TIMERINFO
#undef CONFIG_EXTPOWER
#undef CONFIG_KEYBOARD_KSO_BASE
#undef CONFIG_FLASH
#undef CONFIG_FMAP
#undef CONFIG_LID_SWITCH
#undef CONFIG_SWITCH
/* TODO: Watch Dog is supported but temporarily removed. Currently under 
 * development on KBL and will be carried over to WHL.
 */
#undef CONFIG_WATCHDOG

/* Modules we want to exclude */
#undef CONFIG_CMD_ACCELS
#undef CONFIG_CMD_HASH
#undef CONFIG_CMD_TEMP_SENSOR
#undef CONFIG_CMD_TIMERINFO
#undef CONFIG_ADC
#undef CONFIG_SHA256

#ifndef __ASSEMBLER__

#include "gpio_signal.h"
#include "registers.h"

#endif /* !__ASSEMBLER__ */

#endif /* __CROS_EC_BOARD_H */
