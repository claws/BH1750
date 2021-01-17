/*

Example of BH1750 library usage.

This example initialises two BH1750 objects using different TwoWire
instances (Wire and Wire1) and then makes a light level reading every second.
This is the case for boards such as the ESP8266 and ESP32

Connections:

  BH1750 A:
    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (19 in this example)
    SDA -> SDA (18 in this example)
    ADD -> (not connected) or GND

  BH1750 B:
    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (22 in this example)
    SDA -> SDA (21 in this example)
    ADD -> (not connected) or GND

ADD pin is used to set sensor I2C address. If it has voltage greater or equal
to 0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address
will be 0x23 (by default).

*/

#include "BH1750.h"
#include "Wire.h"

BH1750 bh1750_a;
BH1750 bh1750_b;

void setup() {
  Serial.begin(115200);
  Wire.begin(18, 19);
  Wire1.begin(21, 22);
  bh1750_a.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire);
  bh1750_b.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire1);
}

int error_counter_1_a = 0;
int error_counter_2_a = 0;
int error_counter_1_b = 0;
int error_counter_2_b = 0;

void loop() {
  float light_level_a;
  if (bh1750_a.measurementReady()) {
    light_level_a = bh1750_a.readLightLevel();
  }
  float light_level_b;
  if (bh1750_b.measurementReady()) {
    light_level_b = bh1750_b.readLightLevel();
  }

  if (lround(light_level_a) == -1) {
    error_counter_1_a++;
  }
  if (lround(light_level_a) == -2) {
    error_counter_2_a++;
  }
  if (lround(light_level_b) == -1) {
    error_counter_1_b++;
  }
  if (lround(light_level_b) == -2) {
    error_counter_2_b++;
  }
  Serial.printf("A: %.0f lux %d:%d :: B: %.0f lux %d:%d\n", light_level_a,
                error_counter_1_a, error_counter_2_a, light_level_b,
                error_counter_1_b, error_counter_2_b);
  delay(1000);
}
