# High autonomy automatic GPS tracker.
This project aims to provides an autonomous GPS tracker based on an Arduino as the central unit. Battery saving has been incorporated into every step of the design.

# Features
 * High autonomy
 * Automatic acquisition of GPS position
 * Dynamically adjusted positions acquisition frequency based on speed and time of the day
 * Local buffer of 4095 positions
 * Backup positions using GPRS network and HTTP(s) POST requests

# Requirements

## Hardware
The project has been build on a ATMega328p for its ease of use, low power consumption and wide support, being at the core of the Arduino UNO.

Appart from the main ATMega328p, several chips are needed :
* [SIM808](https://simcom.ee/documents/?dir=SIM808) : GPRS & GPS module.
* [DS3231](https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS3231.html) : Real Time Clock, used for time keeping and battery saving.
* [24LCxxx](https://www.microchip.com/wwwproducts/en/en010828) : EEPROM used to buffer raw data before sending.

## Using standard Arduino boards
This project can be build using standard Arduino boards and shields. For instance, you can start from an Arduino UNO with an Adafruit FONA shield.
This should only be done for testing and debugging purpose though, as both are not designed with energy saving in mind, and will rapidly consumes all your battery life.

## Using a dedicated board
For optimum energy management and maximum autonomy, a double-sided, surface mounted [circuit board]() has been engineered. Taking that path requires either using soldering paste or some fine soldering skills : the SIM808 package can be quite hard to solder by hand.

## Software
The project has been build using Arduino 1.8.5. Ditching Arduino IDE in favor of an advanced IDE (VS Code, Visual Micro) is strongly recommended.

### Arduino Librairies
 * [SIM808]()
 * [uDS3231]()
 * [E24]()
 * [Low-Power](https://github.com/rocketscream/Low-Power)
 * [Arduino-Log](https://github.com/thijse/Arduino-Log/)

## Configuration
To fully use the project, you will need to adjust some values based on your needs. Each file under [src/config](src/config) can be modified, but the one you'll be most likely interested in are :
 * [Alerts.h](/src/config/Alerts.h) : Alerts level and contact phone number
 * [BackupNetwork.h](/src/config/BackupNetwork.h) : Network backup activation and configuration
 * [Sleeps.h](/src/config/Pins.h) : Sleeps duration configuration

# How does it work ?
Each traditional `loop` execution procedes through the following phases :
 * Acquisition of current GPS position
 * Triggering alerts
 * Backuping positions

Each one of these phases requires long waits (acquiring GPS signal, registering to the networks, etc). To preserve battery during those waits, all non-essentials devices are turned off, including the main ATMega328p. An interrupt coming from the RTC DS3231 chip (powered by & 3V coin battery) is used to woke up the main unit.

## Backuping positions
As positions backup can take quite some time, they're only backup in two cases :
 * When stopping for a long time
 * When stopped and many positions are buffered

This behaviour prevent the backup from distrupting the rate at which positions will be acquired.

# Limitations

## Adding features
The downside of using an ATMega328p is a relativly limited space. That space gets filled almost completly by the project in its current state, leaving very few possibilities for further improvments and features. Even adding Serial debugging statements can sometimes be impossible.

## Using an SD card
An SD card was firt intended to be used for logging, data saving and configuration purposes. Due to the limited space available on an ATMega328p (32k), it has been removed early in the development process. Parts of the intended code (using [SdFat](https://github.com/greiman/SdFat)) is still in the repository, buts its use has been disabled.