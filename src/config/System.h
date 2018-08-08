#pragma once

/**
 \def CONFIG_ADDR
 Address of the config block in the I2C EEPROM chip.
 */
#define CONFIG_ADDR 0
/**
 \def CONFIG_RESERVED_SIZE
 Reserved size for the config block in the I2C EEPROM chip.
 */
#define CONFIG_RESERVED_SIZE 128
/**
 \def CONFIG_SEED
 Seed use to detect invalid or outdate configuration data.
 Changing this value will reset the configuration block.
 */
#define CONFIG_SEED 14
/**
 \def VERSION
 Version string, only used for indicative purpose
 */
#define VERSION "1.20"


/**
 \def MENU_TIMEOUT
 When hooked up on Serial port, determine how much milliseconds
 to wait for a user input before proceeding.
 */
#if _DEBUG
#define MENU_TIMEOUT 0
#else
#define MENU_TIMEOUT 20000
#endif

#define MENU_INTERMEDIATE_TIMEOUT 50