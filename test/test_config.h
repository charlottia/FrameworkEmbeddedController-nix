/* Copyright 2013 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Per-test config flags */

#ifndef __TEST_TEST_CONFIG_H
#define __TEST_TEST_CONFIG_H

/* Test config flags only apply for test builds */
#ifdef TEST_BUILD

/* Host commands are sorted. */
#define CONFIG_HOSTCMD_SECTION_SORTED

/* Don't compile features unless specifically testing for them */
#undef CONFIG_VBOOT_HASH
#undef CONFIG_USB_PD_LOGGING

#ifdef TEST_AES
#define CONFIG_AES
#define CONFIG_AES_GCM
#endif

#ifdef TEST_BASE32
#define CONFIG_BASE32
#endif

#ifdef TEST_BKLIGHT_LID
#define CONFIG_BACKLIGHT_LID
#endif

#ifdef TEST_BKLIGHT_PASSTHRU
#define CONFIG_BACKLIGHT_LID
#define CONFIG_BACKLIGHT_REQ_GPIO GPIO_PCH_BKLTEN
#endif

#ifdef TEST_FLASH_LOG
#define CONFIG_CRC8
#define CONFIG_FLASH_ERASED_VALUE32 (-1U)
#define CONFIG_FLASH_LOG
#define CONFIG_FLASH_LOG_BASE (CONFIG_PROGRAM_MEMORY_BASE + 0x800)
#define CONFIG_FLASH_LOG_SPACE 0x800
#define CONFIG_MALLOC
#endif

#ifdef TEST_KB_8042
#define CONFIG_KEYBOARD_PROTOCOL_8042
#endif

#ifdef TEST_KB_MKBP
#define CONFIG_KEYBOARD_PROTOCOL_MKBP
#define CONFIG_MKBP_EVENT
#define CONFIG_MKBP_USE_GPIO
#endif

#ifdef TEST_KB_SCAN
#define CONFIG_KEYBOARD_PROTOCOL_MKBP
#define CONFIG_MKBP_EVENT
#define CONFIG_MKBP_USE_GPIO
#endif

#ifdef TEST_MATH_UTIL
#define CONFIG_MATH_UTIL
#endif

#ifdef TEST_FLOAT
#define CONFIG_FPU
#define CONFIG_MAG_CALIBRATE
#endif

#ifdef TEST_FP
#undef CONFIG_FPU
#define CONFIG_MAG_CALIBRATE
#endif

#ifdef TEST_FPSENSOR
#define CONFIG_AES
#define CONFIG_AES_GCM
#define CONFIG_ROLLBACK_SECRET_SIZE 32
#define CONFIG_SHA256
#endif

#if defined(TEST_MOTION_LID) || defined(TEST_MOTION_ANGLE) || \
	defined(TEST_MOTION_ANGLE_TABLET)
enum sensor_id {
	BASE,
	LID,
	SENSOR_COUNT,
};

#define CONFIG_LID_ANGLE
#define CONFIG_LID_ANGLE_SENSOR_BASE BASE
#define CONFIG_LID_ANGLE_SENSOR_LID LID
#define CONFIG_TABLET_MODE
#define CONFIG_MOTION_FILL_LPC_SENSE_DATA

#endif

#if defined(TEST_MOTION_ANGLE)
#define CONFIG_ACCEL_FORCE_MODE_MASK \
	((1 << CONFIG_LID_ANGLE_SENSOR_BASE) | \
	 (1 << CONFIG_LID_ANGLE_SENSOR_LID))
#define CONFIG_ACCEL_STD_REF_FRAME_OLD
#endif

#if defined(TEST_MOTION_ANGLE_TABLET)
#define CONFIG_ACCEL_FORCE_MODE_MASK \
	((1 << CONFIG_LID_ANGLE_SENSOR_BASE) | \
	 (1 << CONFIG_LID_ANGLE_SENSOR_LID))
#endif

#ifdef TEST_RMA_AUTH

/* Test server public and private keys */
#define RMA_KEY_BLOB	{					\
		0x03, 0xae, 0x2d, 0x2c, 0x06, 0x23, 0xe0, 0x73, \
		0x0d, 0xd3, 0xb7, 0x92, 0xac, 0x54, 0xc5, 0xfd,	\
		0x7e, 0x9c, 0xf0, 0xa8, 0xeb, 0x7e, 0x2a, 0xb5,	\
		0xdb, 0xf4, 0x79, 0x5f, 0x8a, 0x0f, 0x28, 0x3f, \
		0x10						\
	}

#define RMA_TEST_SERVER_PRIVATE_KEY {				\
		0x47, 0x3b, 0xa5, 0xdb, 0xc4, 0xbb, 0xd6, 0x77, \
		0x20, 0xbd, 0xd8, 0xbd, 0xc8, 0x7a, 0xbb, 0x07,	\
		0x03, 0x79, 0xba, 0x7b, 0x52, 0x8c, 0xec, 0xb3,	\
		0x4d, 0xaa, 0x69, 0xf5, 0x65, 0xb4, 0x31, 0xad}
#define RMA_TEST_SERVER_KEY_ID 0x10

#define CONFIG_BASE32
#define CONFIG_CURVE25519
#define CONFIG_RMA_AUTH
#define CONFIG_RNG
#define CONFIG_SHA256
#define CC_EXTENSION CC_COMMAND

#endif

#ifdef TEST_CRC32
#define CONFIG_SW_CRC
#endif

#ifdef TEST_RSA
#define CONFIG_RSA
#define CONFIG_RSA_KEY_SIZE 2048
#define CONFIG_RWSIG_TYPE_RWSIG
#endif

#ifdef TEST_RSA3
#define CONFIG_RSA
#undef CONFIG_RSA_KEY_SIZE
#define CONFIG_RSA_KEY_SIZE 2048
#define CONFIG_RSA_EXPONENT_3
#define CONFIG_RWSIG_TYPE_RWSIG
#endif

#ifdef TEST_SHA256
#define CONFIG_SHA256
#endif

#ifdef TEST_SHA256_UNROLLED
#define CONFIG_SHA256
#define CONFIG_SHA256_UNROLLED
#endif

#ifdef TEST_SHMALLOC
#define CONFIG_MALLOC
#endif

#ifdef TEST_SBS_CHARGING_V2
#define CONFIG_BATTERY
#define CONFIG_BATTERY_MOCK
#define CONFIG_BATTERY_SMART
#define CONFIG_CHARGER
#define CONFIG_CHARGER_V2
#define CONFIG_CHARGER_PROFILE_OVERRIDE
#define CONFIG_CHARGER_INPUT_CURRENT 4032
#define CONFIG_CHARGER_DISCHARGE_ON_AC
#define CONFIG_CHARGER_DISCHARGE_ON_AC_CUSTOM
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
int board_discharge_on_ac(int enabled);
#define I2C_PORT_MASTER 0
#define I2C_PORT_BATTERY 0
#define I2C_PORT_CHARGER 0
#endif

#ifdef TEST_THERMAL
#define CONFIG_CHIPSET_CAN_THROTTLE
#define CONFIG_FANS 1
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
#define CONFIG_TEMP_SENSOR
#define CONFIG_THROTTLE_AP
#define CONFIG_THERMISTOR
#define CONFIG_THERMISTOR_NCP15WB
#define I2C_PORT_THERMAL 0
int ncp15wb_calculate_temp(uint16_t adc);
#endif

#ifdef TEST_FAN
#define CONFIG_FANS 1
#endif

#ifdef TEST_BUTTON
#define CONFIG_KEYBOARD_PROTOCOL_8042
#define CONFIG_VOLUME_BUTTONS
#endif

#ifdef TEST_BATTERY_GET_PARAMS_SMART
#define CONFIG_BATTERY_MOCK
#define CONFIG_BATTERY_SMART
#define CONFIG_CHARGER_INPUT_CURRENT 4032
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
#define I2C_PORT_MASTER 0
#define I2C_PORT_BATTERY 0
#define I2C_PORT_CHARGER 0
#endif

#ifdef TEST_CEC
#define CONFIG_CEC
#endif

#ifdef TEST_LIGHTBAR
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
#define I2C_PORT_LIGHTBAR 0
#define CONFIG_ALS_LIGHTBAR_DIMMING 0
#endif

#if defined(TEST_USB_SM_FRAMEWORK_H3)
#define CONFIG_USB_PD_PORT_COUNT 1
#undef CONFIG_USB_PRL_SM
#undef CONFIG_USB_PE_SM
#undef CONFIG_USB_TYPEC_SM
#undef  CONFIG_SM_NESTING_NUM
#define CONFIG_SM_NESTING_NUM 3
#define CONFIG_USB_SM_FRAMEWORK
#endif

#if defined(TEST_USB_SM_FRAMEWORK_H2)
#define CONFIG_USB_PD_PORT_COUNT 1
#undef CONFIG_USB_PRL_SM
#undef CONFIG_USB_PE_SM
#undef CONFIG_USB_TYPEC_SM
#undef  CONFIG_SM_NESTING_NUM
#define CONFIG_SM_NESTING_NUM 2
#define CONFIG_USB_SM_FRAMEWORK
#endif

#if defined(TEST_USB_SM_FRAMEWORK_H1)
#define CONFIG_USB_PD_PORT_COUNT 1
#undef CONFIG_USB_PRL_SM
#undef CONFIG_USB_PE_SM
#undef CONFIG_USB_TYPEC_SM
#undef  CONFIG_SM_NESTING_NUM
#define CONFIG_SM_NESTING_NUM 1
#define CONFIG_USB_SM_FRAMEWORK
#endif

#if defined(TEST_USB_SM_FRAMEWORK_H0)
#define CONFIG_USB_PD_PORT_COUNT 1
#undef CONFIG_USB_PRL_SM
#undef CONFIG_USB_PE_SM
#undef CONFIG_USB_TYPEC_SM
#undef  CONFIG_SM_NESTING_NUM
#define CONFIG_SM_NESTING_NUM 0
#define CONFIG_USB_SM_FRAMEWORK
#endif

#if defined(TEST_USB_PRL)
#undef  CONFIG_SM_NESTING_NUM
#define CONFIG_SM_NESTING_NUM 3
#define CONFIG_USB_PD_PORT_COUNT 2
#define CONFIG_USB_SM_FRAMEWORK
#undef CONFIG_USB_PE_SM
#undef CONFIG_USB_TYPEC_SM
#define CONFIG_USB_PRL_SM
#define CONFIG_USB_PD_TCPC
#define CONFIG_USB_PD_TCPM_STUB
#define CONFIG_USB_POWER_DELIVERY
#define CONFIG_SHA256
#define CONFIG_SW_CRC
#endif

#if defined(TEST_USB_TYPEC_VPD) || defined(TEST_USB_TYPEC_CTVPD)
#if defined(TEST_USB_TYPEC_VPD)
#define CONFIG_USB_TYPEC_VPD
#else
#define CONFIG_USB_TYPEC_CTVPD
#endif
#undef  CONFIG_SM_NESTING_NUM
#define CONFIG_SM_NESTING_NUM 3

#define CONFIG_USB_PID 0x5036
#define VPD_HW_VERSION 0x0001
#define VPD_FW_VERSION 0x0001
#define USB_BCD_DEVICE 0

/* Vbus impedance in milliohms */
#define VPD_VBUS_IMPEDANCE 65

/* GND impedance in milliohms */
#define VPD_GND_IMPEDANCE 33

#define CONFIG_USB_PD_PORT_COUNT 1
#define CONFIG_USB_SM_FRAMEWORK
#define CONFIG_USB_PE_SM
#define CONFIG_USB_PRL_SM
#define CONFIG_USB_TYPEC_SM
#define CONFIG_USB_PD_TCPC
#define CONFIG_USB_PD_TCPM_STUB
#define CONFIG_USB_POWER_DELIVERY
#define CONFIG_SHA256
#define CONFIG_SW_CRC
#endif /* TEST_USB_TYPEC_VPD or TEST_USB_TYPEC_CTVPD */

#if defined(TEST_USB_PD) || defined(TEST_USB_PD_GIVEBACK) || \
	defined(TEST_USB_PD_REV30)
#define CONFIG_USB_POWER_DELIVERY
#define CONFIG_USB_PD_DUAL_ROLE
#define CONFIG_USB_PD_PORT_COUNT 2
#define CONFIG_USB_PD_TCPC
#define CONFIG_USB_PD_TCPM_STUB
#define CONFIG_SHA256
#define CONFIG_SW_CRC
#ifdef TEST_USB_PD_REV30
#define CONFIG_USB_PD_REV30
#define CONFIG_USB_PID 0x5000
#endif
#ifdef TEST_USB_PD_GIVEBACK
#define CONFIG_USB_PD_GIVE_BACK
#endif
#endif /* TEST_USB_PD || TEST_USB_PD_GIVEBACK || TEST_USB_PD_REV30 */

#if defined(TEST_CHARGE_MANAGER) || defined(TEST_CHARGE_MANAGER_DRP_CHARGING)
#define CONFIG_CHARGE_MANAGER
#define CONFIG_USB_PD_DUAL_ROLE
#define CONFIG_USB_PD_PORT_COUNT 2
#define CONFIG_BATTERY
#define CONFIG_BATTERY_SMART
#define CONFIG_I2C
#define CONFIG_I2C_MASTER
#define I2C_PORT_BATTERY 0
#endif /* TEST_CHARGE_MANAGER_* */

#ifdef TEST_CHARGE_MANAGER_DRP_CHARGING
#define CONFIG_CHARGE_MANAGER_DRP_CHARGING
#else
#undef CONFIG_CHARGE_MANAGER_DRP_CHARGING
#endif /* TEST_CHARGE_MANAGER_DRP_CHARGING */

#ifdef TEST_CHARGE_RAMP
#define CONFIG_CHARGE_RAMP_SW
#define CONFIG_USB_PD_PORT_COUNT 2
#endif

#if defined(TEST_NVMEM) || defined(TEST_NVMEM_VARS)
#define CONFIG_CRC8
#define CONFIG_FLASH_ERASED_VALUE32 (-1U)
#define CONFIG_FLASH_LOG
#define CONFIG_FLASH_LOG_BASE CONFIG_PROGRAM_MEMORY_BASE
#define CONFIG_FLASH_LOG_SPACE 0x800
#define CONFIG_FLASH_NVMEM
#define CONFIG_FLASH_NVMEM_OFFSET_A 0x3d000
#define CONFIG_FLASH_NVMEM_OFFSET_B 0x7d000
#define CONFIG_FLASH_NVMEM_BASE_A                                              \
	(CONFIG_PROGRAM_MEMORY_BASE + CONFIG_FLASH_NVMEM_OFFSET_A)
#define CONFIG_FLASH_NVMEM_BASE_B                                              \
	(CONFIG_PROGRAM_MEMORY_BASE + CONFIG_FLASH_NVMEM_OFFSET_B)
#define CONFIG_FLASH_NEW_NVMEM_BASE_A (CONFIG_FLASH_NVMEM_BASE_A + 0x800)
#define CONFIG_FLASH_NEW_NVMEM_BASE_B (CONFIG_FLASH_NVMEM_BASE_B + 0x800)
#define CONFIG_MALLOC
/* This is legacy NVMEM partition size. */
#define NVMEM_PARTITION_SIZE 0x3000
#define NEW_FLASH_HALF_NVMEM_SIZE                                              \
	(NVMEM_PARTITION_SIZE - CONFIG_FLASH_BANK_SIZE)
#define NEW_NVMEM_PARTITION_SIZE (NVMEM_PARTITION_SIZE - CONFIG_FLASH_BANK_SIZE)
#define NEW_NVMEM_TOTAL_PAGES                                                  \
	(2 * NEW_NVMEM_PARTITION_SIZE / CONFIG_FLASH_BANK_SIZE)
#define CONFIG_SW_CRC
#define CONFIG_FLASH_NVMEM_VARS

#ifndef __ASSEMBLER__
enum nvmem_users { NVMEM_TPM = 0, NVMEM_CR50, NVMEM_NUM_USERS };
#endif
#endif

#ifdef TEST_PINWEAVER
#define CONFIG_DCRYPTO_MOCK
#define CONFIG_PINWEAVER
#define CONFIG_SHA256
#endif /* TEST_PINWEAVER */

#ifdef TEST_RTC
#define CONFIG_HOSTCMD_RTC
#endif

#ifdef TEST_VBOOT
#define CONFIG_RWSIG
#define CONFIG_SHA256
#define CONFIG_RSA
#define CONFIG_RWSIG_TYPE_RWSIG
#define CONFIG_RW_B
#define CONFIG_RW_B_MEM_OFF		CONFIG_RO_MEM_OFF
#undef  CONFIG_RO_SIZE
#define CONFIG_RO_SIZE			(CONFIG_FLASH_SIZE / 4)
#undef  CONFIG_RW_SIZE
#define CONFIG_RW_SIZE			CONFIG_RO_SIZE
#define CONFIG_RW_A_STORAGE_OFF		CONFIG_RW_STORAGE_OFF
#define CONFIG_RW_B_STORAGE_OFF		(CONFIG_RW_A_STORAGE_OFF + \
					 CONFIG_RW_SIZE)
#define CONFIG_RW_A_SIGN_STORAGE_OFF	(CONFIG_RW_A_STORAGE_OFF + \
					 CONFIG_RW_SIZE - CONFIG_RW_SIG_SIZE)
#define CONFIG_RW_B_SIGN_STORAGE_OFF	(CONFIG_RW_B_STORAGE_OFF + \
					 CONFIG_RW_SIZE - CONFIG_RW_SIG_SIZE)
#endif

#ifdef TEST_X25519
#define CONFIG_CURVE25519
#endif /* TEST_X25519 */

#endif  /* TEST_BUILD */
#endif  /* __TEST_TEST_CONFIG_H */
