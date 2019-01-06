# High autonomy GPS tracker.
[![Build Status](https://travis-ci.org/blemasle/gpstracker-firmware.svg?branch=master)](https://travis-ci.org/blemasle/gpstracker-firmware)
[![License](https://img.shields.io/badge/license-MIT%20License-blue.svg)](http://doge.mit-license.org)

This project aims to provides a GPS tracker based on an Arduino Pro mini as the central unit. Battery saving has been incorporated into every step of the design. A custom designed [PCB](https://github.com/blemasle/gpstracker-pcb) is available to avoid using poorly power efficient boards.

# Features
 * High autonomy (several weeks with a 1700 mAh battery)
 * Automatic acquisition of GPS position
 * Dynamically adjusted positions acquisition frequency based on speed and time of the day
 * Local buffer of 4095 positions with a 512K EEPROM
 * Automatic positions backup to any HTTP server using a GPRS powered network

# Requirements

## Hardware
This project has been build on a [ATMega328p](https://www.microchip.com/wwwproducts/en/ATMEGA328P) for its ease of use, low power consumption and wide support.

On top of that, the several ICs are needed :
* [SIM808](https://simcom.ee/documents/?dir=SIM808) : GPRS & GPS.
* [DS3231](https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html) : Real Time Clock, used for time keeping and battery saving.
* [24LCxxx](https://www.microchip.com/wwwproducts/en/en010828) : EEPROM used to buffer raw data before sending.

## Using standard Arduino boards
This project can be build using standard Arduino boards and shields. For instance, you can start from an Arduino UNO with an Adafruit FONA shield.
This should only be done for testing and debugging purpose though, as both are not designed with energy saving in mind, and will rapidly consumes all your battery.

## Using a dedicated board
For optimum energy management and maximum autonomy, a double-sided, surface mounted [PCB](https://github.com/blemasle/gpstracker-pcb) has been created. Taking that path requires either using soldering paste or some fine soldering skills : the SIM808 IC can be quite hard to solder by hand.

## Software
The project has been build using Arduino 1.8.5. Ditching Arduino IDE in favor of an advanced IDE (VS Code, Visual Micro) is strongly recommended.

### Arduino libraries
Install the required libraries through the Arduino's Library Manager UI, or run the following script :
```sh
#!/bin/sh

arduino --install-library "E24"
arduino --install-library "Low-Power"
arduino --install-library "ArduinoLog"
arduino --install-library "MD_DS3231"
arduino --install-library "SIM808"
```

Disable unneeded features of MD_DS3231 for the HEX to fit in the ATMega328p by setting `ENABLE_12H`, `ENABLE_DOW` and `ENABLE_DYNAMIC_CENTURY` to `0` in [MD_DS3231.h](https://github.com/MajicDesigns/MD_DS3231/blob/master/src/MD_DS3231.h) or running `sed -E "s/^#define ENABLE_(12H|DOW|DYNAMIC_CENTURY) 1/#define ENABLE_\1 0/" -i MD_DS3231.h`

### Configuration
To fully use the project, you will need to adjust some values based on your needs. Each file under [src/config](src/config) can be modified, but the one you'll be most likely interested in are :
 * [User.h](/src/config/User.h) : Dumb, versionned file containing an example of the values in [Sensitive.h](/src/config/Sensitive.h)
 * Sensitive.h : Sensitive data that should *never* be versionned (phone number, web server address)
 * [Alerts.h](/src/config/Alerts.h) : Alerts levels
 * [BackupNetwork.h](/src/config/BackupNetwork.h) : Network backup and configuration
 * [Sleeps.h](/src/config/Pins.h) : Sleeps duration configuration

# Compiling
If you're using the [custom board](https://github.com/blemasle/gpstracker-pcb), set the board to `Arduino Pro or Pro Mini` and the processor to `ATMega328P (3.3V, 8Mhz)` before hitting `Verify` or `upload`.
Or simply run `arduino --verify --board arduino:avr:pro:cpu=8MHzatmega328 src/GpsTracker.ino`

# How does it work ?
Each `loop` execution procedes (roughly) through the following phases :
 * Acquisition of current GPS position
 * Triggering alerts if needed
 * Backuping positions if needed
 * Deep sleep until the next `loop`

Each one of these phases requires long waits (acquiring GPS signal, registering to the networks, etc). To preserve battery during those waits, all non-essentials devices are turned off, including the main ATMega328p. An interrupt coming from the RTC DS3231 chip (powered by the 3V coin battery) is used to woke up the main unit.

## Backuping positions
As positions backup can take quite some time, they're only backuped in two cases :
 * When stopping for a long time
 * When stopped and many positions are not yet backuped

This behaviour prevent the backup from distrupting the rate at which positions will be acquired, and avoid the device to look for the GPRS network while moving, which in some case can be difficult. As a downside, it is obvious that the positions are not backuped in realtime.

# Limitations

## Adding features
The downside of using an ATMega328p is a relativly limited space. That space gets filled almost completly by the project in its current state, leaving very few possibilities for further improvments and features. Even adding Serial debugging statements can sometimes be impossible.

## Using an SD card
An SD card was first intended to be used for logging, data saving and configuration purposes. Due to the limited space available on an ATMega328p (32k), it has been removed early in the development process. Parts of the intended code (using [SdFat](https://github.com/greiman/SdFat)) is still in the repository, buts its use has been disabled.

## HTTP
Sadly, the SIM808 HTTP stack is unable to achieve reliable HTTPS. Currently, the backup is done with HTTP requests and there is no encryption, signature or authentication header (because without HTTPS, authentication header serves almost no purpose).
Any information on how to achieve proper and reliable HTTPS support or at least request signature (with the limited resource of an ATMega328p of course) is welcomed.

## Several firmwares
One possibility to include additional features would be to split the current firmware into several ones. For instance :
 - one "main" firmware
 - one "configuration" firmware to read and write EEPROM configuration rather than hardcoded values into the header files
 - one "debugging" firmware with additional logging and the ability to run specific parts of the main `loop` indepently

As I was using and needed the device *while* developing it, I couldn't take such a big risk that would potentionally break things and miss or lost some positions records.