/*

  This is a library for the BH1750FVI Digital Light Sensor breakout board.

  The BH1750 board uses I2C for communication. Two pins are required to
  interface to the device. Configuring the I2C bus is expected to be done
  in user code. The BH1750 library doesn't do this automatically.

  Datasheet:
  http://www.elechouse.com/elechouse/images/product/Digital%20light%20Sensor/bh1750fvi-e.pdf

  Written by Christopher Laws, March, 2013.

*/

#ifndef BH1750messages_h
#define BH1750messages_h

/*
  A default style of the debug messages, to modify a look of the debug message
  or to use a custom command just define own BH1750_LOG command macro

  Template for your custom format (replace square brackets by your own text):
  #  define BH1750_LOG(fmt, ...) Serial.printf(                              \
      "[your formatted prefix]" fmt "[your formatted suffix]",               \
      [[your prefix arg_1], [your prefix arg_2], ..., [your prefix arg_n],]  \
      ##__VA_ARGS__                                                          \
      [, [your suffix arg_1], [your suffix arg_2], ..., [your suffix arg_n]] \
      )

  Default output example (without build in console timestamp):
          24.358 sec. -> [BH1750] Error: Invalid mode
        3920.563 sec. -> [BH1750] Raw value: 32568.50

  Default output example (with build in console timestamp):
  17:28:32.478 ->         24.358 sec. -> [BH1750] Error: Invalid mode
  17:28:32.478 ->       3920.563 sec. -> [BH1750] Raw value: 32568.50
*/
#ifndef BH1750_LOG
#  define BH1750_LOG(fmt, ...) Serial.printf(       \
      "% 9.3f sec. -> [BH1750] " fmt "\n",          \
      (double)millis() / 1000.0,                    \
      ##__VA_ARGS__                                 \
      )
#endif // !BH1750_LOG

// This error message shows when invalid measurement mode is selected
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_INVALID_MODE
#    define BH1750_INVALID_MODE         "Error: Invalid mode"
#endif // !BH1750_INVALID_MODE

// This error message shows when buffer of the Wire bus has not insufficient space to send data
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_BUFFER_OVERFLOW
#  define BH1750_BUFFER_OVERFLOW        "Error: Data are too long for transmit buffer"
#endif // !BH1750_BUFFER_OVERFLOW

// This error message shows when the Wire bus cannot reach the sensor 
// (e.g. sensor is disconnected or was used wrong address)
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_ADDRESS_NACK
#    define BH1750_ADDRESS_NACK         "Error: Received NACK on transmit of address"
#endif // !BH1750_ADDRESS_NACK

// This error message shows when the sensor receives invalid data/command or it's buffer overflows
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_DATA_NACK
#  define BH1750_DATA_NACK              "Error: Received NACK on transmit of data"
#endif // !BH1750_DATA_NACK

// This error message shows when an other error of the Wire bus appears
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_OTHER_ERROR
#  define BH1750_OTHER_ERROR            "Error: Other error of the bus"
#endif // !BH1750_OTHER_ERROR

// This error message shows when an undefined error of the Wire bus appears
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_UNDEFINED_ERROR
#  define BH1750_UNDEFINED_ERROR        "Error: Undefined error of the bus"
#endif // !BH1750_UNDEFINED_ERROR

// This error message shows when entered measurement time is out of allowed range
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_MTREG_OUT_OF_RANGE
#  define BH1750_MTREG_OUT_OF_RANGE     "Error: MTreg value is out of range"
#endif // !BH1750_MTREG_OUT_OF_RANGE

// This error message shows when the sensor has not been initialized yet
// No additional formatting parameters (e.g. %d) allowed
#ifndef BH1750_SENSOR_UNCONFIGURED
#  define BH1750_SENSOR_UNCONFIGURED    "Error: Device is not configured"
#endif // !BH1750_SENSOR_UNCONFIGURED

// The format of sensor's raw value output shown when the debug of values is enabled
// Formatting parameter "%f" must be present in the message
#ifndef BH1750_RAW_VALUE_FORMAT
#  define BH1750_RAW_VALUE_FORMAT       "Raw value: %f"
#endif // !BH1750_RAW_VALUE_FORMAT

// The format of sensor's MTreg factor shown when the debug of values is enabled
// MTreg is defined as ratio of the default MTreg value and setted value
// Formatting parameter "%f" must be present in the message
#ifndef BH1750_MTREG_FACTOR_FORMAT
#  define BH1750_MTREG_FACTOR_FORMAT    "Raw value: %f"
#endif // !BH1750_MTREG_FACTOR_FORMAT

// The format of sensor's MTreg factor shown when the debug of values is enabled
// MTreg is defined as ratio of the default MTreg value and setted value
// Formatting parameter "%f" must be present in the message
#ifndef BH1750_CONVERTED_VALUE_FORMAT
#  define BH1750_CONVERTED_VALUE_FORMAT "Converted value: %f"
#endif // !BH1750_CONVERTED_VALUE_FORMAT

#endif // !BH1750messages_h