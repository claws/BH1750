# BH1750

[![Build Status](https://travis-ci.org/claws/BH1750.svg?branch=master)](https://travis-ci.org/claws/BH1750)<br>

An Arduino library for digital light sensor breakout boards containing the BH1750FVI IC.

The board uses I2C for communication, and two pins are required to interface to the device.

Datasheet can be obtained [here](http://rohmfs.rohm.com/en/products/databook/datasheet/ic/sensor/light/bh1750fvi-e.pdf)

The BH1750 had six different measurment modes. They are divided in two groups;
continuous and one-time measurments. In continuous mode, the sensor continuously
measures lightness value. In one-time mode, sensor makes only one measurment, and
then goes into Power Down mode after this.

Each mode, has three different precisions:

  - Low Resolution Mode - (4 lx precision, 16ms measurment time)
  - High Resolution Mode - (1 lx precision, 120ms measurment time)
  - High Resolution Mode 2 - (0.5 lx precision, 120ms measurment time)

By default, this library uses Continuous High Resolution Mode, but you can set
any other mode by definepassing the mode argument to BH1750.begin().

Remember, if you use One-Time mode, your sensor will go to Power Down mode
each time, when it completes measurment and you've read it.

Typical Connection:

  - VCC -> 5V (3V3 on Arduino Due, Zero, MKR1000, etc)
  - GND -> GND
  - SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due on esp8266 free selectable)
  - SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due on esp8266 free selectable)
  - ADD -> GND or VCC (see below)

ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
be 0x23 (by default).


## Installation

Click "Clone or download" -> "Download ZIP" button.

  - **(For Arduino >= 1.5.x)** Use the way above, or Library Manager. Open Arduino
    IDE, click `Sketch -> Include library -> Add .ZIP library ` and select the
    downloaded archive.

  - **(For Arduino < 1.5.x)** Extract the archive to
    ``<Your User Directory>/My Documents/Arduino/libraries/`` folder and rename it
    to `BH1750`. Restart IDE.

Additional info, about library installation process - https://www.arduino.cc/en/Guide/Libraries


## Examples

There are two examples in the examples directory. One shows a simple use while
the other shows a more advanced use case.
