PATH_ARDUINO=/Users/dizzypotato/Library/Arduino15/packages
PATH_ESP=$PATH_ARDUINO/esp8266/hardware/esp8266/2.4.1

LIB_ESP8266=$PATH_ESP/cores/esp8266
LIB_ESPGENERIC=$PATH_ESP/variants/generic
LIB_SDK=$PATH_ESP/tools/sdk/include
LIB_ESP8266_WIFI=$PATH_ESP/libraries/ESP8266WiFi/src

docker run --rm -v "$PWD":/usr/src/myapp \
  -v $LIB_ESP8266:/lib/esp8266 \
  -v $LIB_ESP8266_WIFI:/lib/esp8266lib \
  -v $LIB_SDK:/lib/sdk \
  -v $LIB_ESPGENERIC:/lib/espgeneric \
  -w /usr/src/myapp gcc:latest \

  gcc -I /lib/esp8266 -I /lib/esp8266lib -I /lib/sdk -I /lib/espgeneric -o esbot.bin esbot.cpp
