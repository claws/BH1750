/*

  Different I2C address BH1750 library usage example

  This example has some comments about advanced usage features.

  Connection:

    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on
  esp8266 free selectable) SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on
  Mega and Due, on esp8266 free selectable) ADD -> VCC

  ADD pin is used to set sensor I2C address. If it has voltage greater or equal
  to 0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address
  will be 0x23 (by default).

*/

#include <BH1750.h>
#include <Wire.h>

/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/
BH1750 lightMeter;

void setup() {

  Serial.begin(9600);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);

  // begin returns a boolean that can be used to detect setup problems.
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5c)) {
    Serial.println(F("BH1750 different I2C address begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop() {

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
}
