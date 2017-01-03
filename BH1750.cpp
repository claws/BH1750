/*

  This is a library for the BH1750FVI Digital Light Sensor
  breakout board.

  The board uses I2C for communication. 2 pins are required to
  interface to the device.

  Written by Christopher Laws, March, 2013.

*/

#include "BH1750.h"

// Define milliseconds delay for ESP8266 platform
#if defined(ESP8266)

  #include <pgmspace.h>
  #define _delay_ms(ms) delayMicroseconds((ms) * 1000)

// Use _delay_ms from utils for AVR-based platforms
#elif defined(__avr__)
  #include <util/delay.h>

// Use Wiring's delay for compability with another platforms
#else
  #define _delay_ms(ms) delay(ms)
#endif


// Legacy Wire.write() function fix
#if (ARDUINO >= 100)
  #define __wire_write(d) Wire.write(d)
#else
  #define __wire_write(d) Wire.send(d)
#endif


// Legacy Wire.read() function fix
#if (ARDUINO >= 100)
  #define __wire_read() Wire.read()
#else
  #define __wire_read() Wire.receive()
#endif


/**
 * Constructor
 * @params addr Sensor address (0x76 or 0x72, see datasheet)
 *
 * On most sensor boards, it was 0x76
 */
BH1750::BH1750(byte addr) {

  BH1750_I2CADDR = addr;

}


/**
 * Begin I2C and configure sensor
 * @param mode Measurment mode
 */
void BH1750::begin(uint8_t mode) {

  // Initialize I2C
  Wire.begin();

  // Configure sensor in specified mode
  configure(mode);

}


/**
 * Configurate BH1750 with specified working mode
 * @param mode Measurment mode
 */
void BH1750::configure(uint8_t mode) {

  // Check, is measurment mode exist
  switch (mode) {

    case BH1750_CONTINUOUS_HIGH_RES_MODE:
    case BH1750_CONTINUOUS_HIGH_RES_MODE_2:
    case BH1750_CONTINUOUS_LOW_RES_MODE:
    case BH1750_ONE_TIME_HIGH_RES_MODE:
    case BH1750_ONE_TIME_HIGH_RES_MODE_2:
    case BH1750_ONE_TIME_LOW_RES_MODE:

      // Send mode to sensor
      Wire.beginTransmission(BH1750_I2CADDR);
      __wire_write((uint8_t)mode);
      Wire.endTransmission();

      // Wait few moments for waking up
      _delay_ms(10);
      break;

    default:

      // Invalid measurement mode
      #ifdef BH1750_DEBUG
        Serial.println(F("BH1750: Invalid measurment mode"));
      #endif

      break;

  }

}


/**
 * Read light level from sensor
 * @return  Lightness in lux (0 ~ 65535)
 */
uint16_t BH1750::readLightLevel(void) {

  // Measurment result will be stored here
  uint16_t level;

  // Read two bytes from sensor
  Wire.requestFrom(BH1750_I2CADDR, 2);

  // Read two bytes, which are low and high parts of sensor value
  level = __wire_read();
  level <<= 8;
  level |= __wire_read();

  // Send raw value if debug enabled
  #ifdef BH1750_DEBUG
    Serial.print(F("[BH1750] Raw value: "));
    Serial.println(level);
  #endif

  // Convert raw value to lux
  level /= 1.2;

  // Send converted value, if debug enabled
  #ifdef BH1750_DEBUG
    Serial.print(F("[BH1750] Converted value: "));
    Serial.println(level);
  #endif

  return level;

}
