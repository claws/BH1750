/*

  Advanced BH1750 library usage example

  This example had some comments about advanced usage features.

  Connection:

    VCC -> 5V (3V3 on Arduino Due, Zero, MKR1000, etc)
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due)
    ADD -> GND or VCC (see below)

  ADD pin uses to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (as example, you've connected it to VCC) - sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) - sensor address will
  be 0x23 (by default).

*/

#include <Wire.h>
#include <BH1750.h>

/*

  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library use 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/
BH1750 lightMeter(0x23);

void setup(){

  Serial.begin(9600);

  /*

    BH1750 had six different measurment modes. They are divided in two groups -
    continuous and one-time measurments. In continuous mode, sensor continuously
    measures lightness value. And in one-time mode, sensor makes only one
    measurment, and going to Power Down mode after this.

    Each mode, has three different precisions:

      - Low Resolution Mode - (4 lx precision, 16ms measurment time)
      - High Resolution Mode - (1 lx precision, 120ms measurment time)
      - High Resolution Mode 2 - (0.5 lx precision, 120ms measurment time)

    By default, library use Continuous High Resolution Mode, but you can set
    any other mode, by define it to BH1750.begin() or BH1750.configure() functions.

    [!] Remember, if you use One-Time mode, your sensor will go to Power Down mode
    each time, when it completes measurment and you've read it.

    Full mode list:

      BH1750_CONTINUOUS_LOW_RES_MODE
      BH1750_CONTINUOUS_HIGH_RES_MODE (default)
      BH1750_CONTINUOUS_HIGH_RES_MODE_2

      BH1750_ONE_TIME_LOW_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE_2

  */

  lightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
  Serial.println(F("BH1750 Test"));

}


void loop() {

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

}
