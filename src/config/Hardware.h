/**
 * Defines external hardware parameters
 */

#include <E24.h>
#include <SoftwareSerial.h>

#define E24_ADDRESS 	E24_ADDR						///< I2C address of the 24xxx chip
#define E24_SIZE		E24Size_t::E24_512K				///< 24xxx chip size

#define SIM808_BAUDRATE 4800							///< Control the baudrate use to communicate with the SIM808 module
#define SIM_SERIAL_TYPE	SoftwareSerial					///< Type of variable that holds the Serial communication with SIM808
#define SIM_SERIAL		SIM_SERIAL_TYPE(SIM_TX, SIM_RX)	///< Definition of the instance that holds the Serial communication with SIM808