/* Copyright 2022 The ChromiumOS Authors.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <zephyr/shell/shell.h>
#include <zephyr/ztest.h>

#include "console.h"
#include "test/drivers/test_state.h"

ZTEST_SUITE(console_cmd_rw, drivers_predicate_post_main, NULL, NULL, NULL,
	    NULL);

ZTEST_USER(console_cmd_rw, test_too_few_args)
{
	zassert_equal(EC_ERROR_PARAM_COUNT,
		      shell_execute_cmd(get_ec_shell(), "rw"), NULL);
}

ZTEST_USER(console_cmd_rw, test_error_param1)
{
	zassert_equal(EC_ERROR_PARAM1,
		      shell_execute_cmd(get_ec_shell(), "rw .j"), NULL);
}