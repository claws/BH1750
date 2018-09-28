/*

  Example of BH1750 library usage.

  This example initialises the BH1750 object using the default high resolution
  continuous mode and then makes a light level reading every second.

  Connection:

    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
    ADD -> (not connected) or GND

  ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
  be 0x23 (by default).

*/

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup(){

  Serial.begin(115200);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);

  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  //lightMeter.setMTreg(69);  //not needed, only mentioning it

  Serial.println(F("BH1750 Test begin"));

}


void loop() {
  //we use here the maxWait option due fail save and we want reduced information loss due missing decimal part
  float lux = lightMeter.readLightLevel(true, true)/100.0;
  if (lux > 40000.0) {
    //reduce time slot - needed in direct sun light
    if(lightMeter.setMTreg(32)) {
      Serial.println(F("[DEBUG]: MTReg low"));
      lux = lightMeter.readLightLevel(true, true)/100.0;
    }
  }
  else {
      if(lux > 10.0 && lightMeter.setMTreg(69)) { 
        Serial.println(F("[DEBUG]: MTReg default"));
        lux = lightMeter.readLightLevel(true, true)/100.0;
      }
  }
  if (lux <= 10.0) {
    //very low light environment
    if(lightMeter.setMTreg(254)) {
      Serial.println(F("[DEBUG]: MTReg high"));
      lux = lightMeter.readLightLevel(true, true)/100.0;
    }
  }
  Serial.print(F("Light: "));
  Serial.print(lux);
  Serial.println(F(" lx"));
  Serial.println(F("--------------------------------------"));
  delay(5000);

}
