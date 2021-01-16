#!/bin/bash
#
# A convenience script to automate building BH1750 examples.
#

# AVR
echo "Compiling examples using FQBN arduino:avr:uno"
arduino-cli compile --fqbn arduino:avr:uno ./examples/BH1750test/BH1750test.ino
arduino-cli compile --fqbn arduino:avr:uno ./examples/BH1750advanced/BH1750advanced.ino
arduino-cli compile --fqbn arduino:avr:uno ./examples/BH1750onetime/BH1750onetime.ino
arduino-cli compile --fqbn arduino:avr:uno ./examples/BH1750autoadjust/BH1750autoadjust.ino

# ESP8266
echo "Compiling examples using FQBN esp8266:esp8266:generic"
arduino-cli core update-index --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
arduino-cli core install esp8266:esp8266 --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
arduino-cli compile --fqbn esp8266:esp8266:generic ./examples/BH1750test/BH1750test.ino
arduino-cli compile --fqbn esp8266:esp8266:generic ./examples/BH1750advanced/BH1750advanced.ino
arduino-cli compile --fqbn esp8266:esp8266:generic ./examples/BH1750onetime/BH1750onetime.ino
arduino-cli compile --fqbn esp8266:esp8266:generic ./examples/BH1750autoadjust/BH1750autoadjust.ino

# ESP32
echo "Compiling examples using FQBN esp32:esp32:esp32"
arduino-cli core update-index --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
arduino-cli core install esp32:esp32 --additional-urls https://dl.espressif.com/dl/package_esp32_index.json
arduino-cli compile --fqbn esp32:esp32:esp32 ./examples/BH1750test/BH1750test.ino
arduino-cli compile --fqbn esp32:esp32:esp32 ./examples/BH1750advanced/BH1750advanced.ino
arduino-cli compile --fqbn esp32:esp32:esp32 ./examples/BH1750onetime/BH1750onetime.ino
arduino-cli compile --fqbn esp32:esp32:esp32 ./examples/BH1750autoadjust/BH1750autoadjust.ino
arduino-cli compile --fqbn esp32:esp32:esp32 ./examples/BH1750two_i2c/BH1750two_i2c.ino
