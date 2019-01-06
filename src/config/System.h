#pragma once

#define CONFIG_ADDR 0				///< Address of the config block in the I2C EEPROM chip.
#define CONFIG_RESERVED_SIZE 128	///< Reserved size for the config block in the I2C EEPROM chip.
#define CONFIG_SEED 14				///< Seed use to detect invalid or outdate configuration data.
#define VERSION "1.44"				/// Version string, only used for indicative purpose.


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