# High autonomy automatic GPS tracker system.
This project aims to provides an autonomous GPS tracker system based on an Arduino as the central unit. Battery saving has been incorporated into every step of the design.

# Features
The device will fetch and save GPS position and device metrics at regular intervals. Those intervals are dynamiccaly adjusted to try to find the best compromise between accurate tracking of a moving target and battery saving.
Additionaly, all those data can be send to an HTTP(S) based web server.

# Requirements

## Hardware
The project has been build on a ATMega328p for its ease of use, low power consumption and wide support, being at the core of the Arduino UNO. The downside is a relativly limited space. That space gets filled almost completly by the project in its current state, leaving very few possibilities for further improvments and features. Even adding Serial debugging statements can sometimes be impossible.

Appart from the main ATMega328p, several chips are needed :
* [SIM808](https://simcom.ee/documents/?dir=SIM808) : GPRS & GPS module.
* [DS3231](https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html) : Real Time Clock, used for time keeping and battery saving.
* [24LCxxx](https://www.microchip.com/wwwproducts/en/en010828) : EEPROM used to buffer raw data before sending.

## Additional Librairies
On top of the hardware librairies, the project use two libraries :
 * [Low-Power](https://github.com/rocketscream/Low-Power) for reliable power management.
 * [Arduino-Log](https://github.com/thijse/Arduino-Log/) for customizable Serial logging.

# Building the hardware

## Using standard Arduino materials
This project can be build using standard Arduino materials and shields. For instance, you can use an Arduino UNO with an Adafruit FONA shield.
This should only be done for testing and debugging purpose though, as both are very energy inefficient, and will rapidly consumes all your battery life.

## Using a dedicated board
For optimum energy consumption, a double-sided, surface mounted [circuit board]() has been engineered. Taking that path requires some fine soldering skills : the SIM808 package can be quite hard to solder by hand.

## Configuration

The projects currently use 9 arduino "named" pins on the ATMega. There are all listed in [Pins.h](/src/Pins.h).

# How does it work ?
TODO : description of the general principles of the tracker. Which components are used and why/when ?

# Limitations
An SD card was firt intended to be used for logging, data saving and configuration purposes. Due to the limited space available on an ATMega328p (32k), it has been removed early in the development process. Parts of the intended code (using [SdFat](https://github.com/greiman/SdFat)) is still in the repository, buts its use has been disabled.