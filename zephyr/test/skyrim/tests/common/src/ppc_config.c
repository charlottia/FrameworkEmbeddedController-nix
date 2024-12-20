/* Copyright 2023 The ChromiumOS Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include <zephyr/drivers/gpio/gpio_emul.h>
#include <zephyr/fff.h>
#include <zephyr/ztest.h>

#include <gpio.h>
#include <usbc_ppc.h>

FAKE_VOID_FUNC(aoz1380_interrupt, int);
FAKE_VOID_FUNC(nx20p348x_interrupt, int);

static void ppc_config_before(void *fixture)
{
	ARG_UNUSED(fixture);
	RESET_FAKE(aoz1380_interrupt);
	RESET_FAKE(nx20p348x_interrupt);
}

void ppc_interrupt(enum gpio_signal signal);
int board_aoz1380_set_vbus_source_current_limit(int port,
						enum tcpc_rp_value rp);

ZTEST_SUITE(ppc_config, NULL, NULL, ppc_config_before, NULL, NULL);

ZTEST(ppc_config, test_board_aoz1380_set_vbus_source_current_limit)
{
	int rv;
	const struct gpio_dt_spec *gpio =
		GPIO_DT_FROM_NODELABEL(ioex_usb_c0_ilim_3a_en);

	/*
	 * board_aoz1380_set_vbus_source_current_limit should set
	 * ioex_usb_c0_ilim_3a_en to 1 for 3A, 0 otherwise.
	 */
	rv = board_aoz1380_set_vbus_source_current_limit(0, TYPEC_RP_3A0);
	zassert_equal(rv, EC_SUCCESS);
	zassert_equal(gpio_emul_output_get(gpio->port, gpio->pin), 1);

	rv = board_aoz1380_set_vbus_source_current_limit(0, TYPEC_RP_1A5);
	zassert_equal(rv, EC_SUCCESS);
	zassert_equal(gpio_emul_output_get(gpio->port, gpio->pin), 0);

	/* Only port0 is supported. */
	rv = board_aoz1380_set_vbus_source_current_limit(1, TYPEC_RP_1A5);
	zassert_equal(rv, EC_ERROR_INVAL);
}

ZTEST(ppc_config, test_ppc_interrupt)
{
	ppc_interrupt(GPIO_USB_C0_PPC_INT_ODL);
	zassert_equal(aoz1380_interrupt_fake.call_count, 1);
	/* port */
	zassert_equal(aoz1380_interrupt_fake.arg0_val, 0);

	ppc_interrupt(GPIO_USB_C1_PPC_INT_ODL);
	zassert_equal(nx20p348x_interrupt_fake.call_count, 1);
	/* port */
	zassert_equal(nx20p348x_interrupt_fake.arg0_val, 1);
}
