/*

  This is a library for the BH1750FVI Digital Light Sensor breakout board.

  The BH1750 board uses I2C for communication. Two pins are required to
  interface to the device. Configuring the I2C bus is expected to be done
  in user code. The BH1750 library doesn't do this automatically.

  Written by Christopher Laws, March, 2013.

*/

#include "BH1750.h"

// Define milliseconds delay for ESP8266 platform
#if defined(ESP8266)

#  include <pgmspace.h>
#  define _delay_ms(ms) delayMicroseconds((ms)*1000)

// Use _delay_ms from utils for AVR-based platforms
#elif defined(__avr__)
#  include <util/delay.h>

// Use Wiring's delay for compability with another platforms
#else
#  define _delay_ms(ms) delay(ms)
#endif

// Legacy Wire.write() function fix
#if (ARDUINO >= 100)
#  define __wire_write(d) I2C->write(d)
#else
#  define __wire_write(d) I2C->send(d)
#endif

// Legacy Wire.read() function fix
#if (ARDUINO >= 100)
#  define __wire_read() I2C->read()
#else
#  define __wire_read() I2C->receive()
#endif

/**
 * Constructor
 * @params addr Sensor address (0x76 or 0x72, see datasheet)
 *
 * On most sensor boards, it was 0x76
 */
BH1750::BH1750(byte addr) {

  BH1750_I2CADDR = addr;
  // Allows user to change TwoWire instance
  I2C = &Wire;
}

/**
 * Configure sensor
 * @param mode Measurement mode
 * @param addr Address of the sensor
 * @param i2c TwoWire instance connected to I2C bus
 */
bool BH1750::begin(Mode mode, byte addr, TwoWire* i2c) {

  // I2C is expected to be initialized outside this library
  // But, allows a different address and TwoWire instance to be used
  if (i2c) {
    I2C = i2c;
  }
  if (addr) {
    BH1750_I2CADDR = addr;
  }

  // Configure sensor in specified mode and set default MTreg
  return (configure(mode) && setMTreg(BH1750_DEFAULT_MTREG));
}

/**
 * Configure BH1750 with specified mode
 * @param mode Measurement mode
 */
bool BH1750::configure(Mode mode) {

  // default transmission result to a value out of normal range
  byte ack = 5;

  // Check measurement mode is valid
  switch (mode) {

  case BH1750::CONTINUOUS_HIGH_RES_MODE:
  case BH1750::CONTINUOUS_HIGH_RES_MODE_2:
  case BH1750::CONTINUOUS_LOW_RES_MODE:
  case BH1750::ONE_TIME_HIGH_RES_MODE:
  case BH1750::ONE_TIME_HIGH_RES_MODE_2:
  case BH1750::ONE_TIME_LOW_RES_MODE:

    // Send mode to sensor
    I2C->beginTransmission(BH1750_I2CADDR);
    __wire_write((uint8_t)mode);
    ack = I2C->endTransmission();

    // Wait a few moments to wake up
    _delay_ms(10);
    break;

  default:
    // Invalid measurement mode
    Serial.println(F("[BH1750] ERROR: Invalid mode"));
    break;
  }

  // Check result code
  switch (ack) {
  case 0:
    BH1750_MODE = mode;
    lastReadTimestamp = millis();
    return true;
  case 1: // too long for transmit buffer
    Serial.println(F("[BH1750] ERROR: too long for transmit buffer"));
    break;
  case 2: // received NACK on transmit of address
    Serial.println(F("[BH1750] ERROR: received NACK on transmit of address"));
    break;
  case 3: // received NACK on transmit of data
    Serial.println(F("[BH1750] ERROR: received NACK on transmit of data"));
    break;
  case 4: // other error
    Serial.println(F("[BH1750] ERROR: other error"));
    break;
  default:
    Serial.println(F("[BH1750] ERROR: undefined error"));
    break;
  }

  return false;
}

/**
 * Configure BH1750 MTreg value
 * MT reg = Measurement Time register
 * @param MTreg a value between 31 and 254. Default: 69
 * @return bool true if MTReg successful set
 * 		false if MTreg not changed or parameter out of range
 */
bool BH1750::setMTreg(byte MTreg) {
  if (MTreg < BH1750_MTREG_MIN || MTreg > BH1750_MTREG_MAX) {
    Serial.println(F("[BH1750] ERROR: MTreg out of range"));
    return false;
  }
  byte ack = 5;
  // Send MTreg and the current mode to the sensor
  //   High bit: 01000_MT[7,6,5]
  //    Low bit: 011_MT[4,3,2,1,0]
  I2C->beginTransmission(BH1750_I2CADDR);
  __wire_write((0b01000 << 3) | (MTreg >> 5));
  ack = I2C->endTransmission();
  I2C->beginTransmission(BH1750_I2CADDR);
  __wire_write((0b011 << 5) | (MTreg & 0b11111));
  ack = ack | I2C->endTransmission();
  I2C->beginTransmission(BH1750_I2CADDR);
  __wire_write(BH1750_MODE);
  ack = ack | I2C->endTransmission();

  // Wait a few moments to wake up
  _delay_ms(10);

  // Check result code
  switch (ack) {
  case 0:
    BH1750_MTreg = MTreg;
    return true;
  case 1: // too long for transmit buffer
    Serial.println(F("[BH1750] ERROR: too long for transmit buffer"));
    break;
  case 2: // received NACK on transmit of address
    Serial.println(F("[BH1750] ERROR: received NACK on transmit of address"));
    break;
  case 3: // received NACK on transmit of data
    Serial.println(F("[BH1750] ERROR: received NACK on transmit of data"));
    break;
  case 4: // other error
    Serial.println(F("[BH1750] ERROR: other error"));
    break;
  default:
    Serial.println(F("[BH1750] ERROR: undefined error"));
    break;
  }

  return false;
}

/**
 * Checks whether enough time has gone to read a new value
 * @param maxWait a boolean if to wait for typical or maximum delay
 * @return a boolean if a new measurement is possible
 *
 */
bool BH1750::measurementReady(bool maxWait) {
  unsigned long delaytime = 0;
  switch (BH1750_MODE) {
  case BH1750::CONTINUOUS_HIGH_RES_MODE:
  case BH1750::CONTINUOUS_HIGH_RES_MODE_2:
  case BH1750::ONE_TIME_HIGH_RES_MODE:
  case BH1750::ONE_TIME_HIGH_RES_MODE_2:
    maxWait ? delaytime = (180 * BH1750_MTreg / (byte)BH1750_DEFAULT_MTREG)
            : delaytime = (120 * BH1750_MTreg / (byte)BH1750_DEFAULT_MTREG);
    break;
  case BH1750::CONTINUOUS_LOW_RES_MODE:
  case BH1750::ONE_TIME_LOW_RES_MODE:
    // Send mode to sensor
    maxWait ? delaytime = (24 * BH1750_MTreg / (byte)BH1750_DEFAULT_MTREG)
            : delaytime = (16 * BH1750_MTreg / (byte)BH1750_DEFAULT_MTREG);
    break;
  default:
    break;
  }
  // Wait for new measurement to be possible.
  // Measurements have a maximum measurement time and a typical measurement
  // time. The maxWait argument determines which measurement wait time is
  // used when a one-time mode is being used. The typical (shorter)
  // measurement time is used by default and if maxWait is set to True then
  // the maximum measurement time will be used. See data sheet pages 2, 5
  // and 7 for more details.
  unsigned long currentTimestamp = millis();
  if (currentTimestamp - lastReadTimestamp >= delaytime) {
    return true;
  } else
    return false;
}

/**
 * Read light level from sensor
 * The return value range differs if the MTreg value is changed. The global
 * maximum value is noted in the square brackets.
 * @return Light level in lux (0.0 ~ 54612,5 [117758,203])
 * 	   -1 : no valid return value
 * 	   -2 : sensor not configured
 */
float BH1750::readLightLevel() {

  if (BH1750_MODE == UNCONFIGURED) {
    Serial.println(F("[BH1750] Device is not configured!"));
    return -2.0;
  }

  // Measurement result will be stored here
  float level = -1.0;

  // Read two bytes from the sensor, which are low and high parts of the sensor
  // value
  if (2 == I2C->requestFrom((int)BH1750_I2CADDR, (int)2)) {
    unsigned int tmp = 0;
    tmp = __wire_read();
    tmp <<= 8;
    tmp |= __wire_read();
    level = tmp;
  }
  lastReadTimestamp = millis();

  if (level != -1.0) {
// Print raw value if debug enabled
#ifdef BH1750_DEBUG
    Serial.print(F("[BH1750] Raw value: "));
    Serial.println(level);
#endif

    if (BH1750_MTreg != BH1750_DEFAULT_MTREG) {
      level *= (float)((byte)BH1750_DEFAULT_MTREG / (float)BH1750_MTreg);
// Print MTreg factor if debug enabled
#ifdef BH1750_DEBUG
      Serial.print(F("[BH1750] MTreg factor: "));
      Serial.println(
          String((float)((byte)BH1750_DEFAULT_MTREG / (float)BH1750_MTreg)));
#endif
    }
    if (BH1750_MODE == BH1750::ONE_TIME_HIGH_RES_MODE_2 ||
        BH1750_MODE == BH1750::CONTINUOUS_HIGH_RES_MODE_2) {
      level /= 2;
    }
    // Convert raw value to lux
    level /= BH1750_CONV_FACTOR;

// Print converted value if debug enabled
#ifdef BH1750_DEBUG
    Serial.print(F("[BH1750] Converted float value: "));
    Serial.println(level);
#endif
  }

  return level;
}
