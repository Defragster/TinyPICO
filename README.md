# TinyPICO
TinyPICO - ESP32 PICO
---------------------
Getting Started: https://www.tinypico.com/gettingstarted

Hardware: https://www.crowdsupply.com/unexpected-maker/tinypico

Github: https://github.com/tinypico/tinypico-arduino : has #include <TinyPICO.h>


SPIFFS_time.ino
-----------------
First sample of combined samples - connects to WiFi {set SSID/PW } gets ntp {adjust timezone}, tests SPIFFS, shows space, prints TEMP/WiFi state each 10 secs and does TinyPICO RGB color change.

SSD1306SimpleDemo.ino
-----------------
Got ssd1306 working with : https://github.com/ThingPulse/esp8266-oled-ssd1306  In \libraries\ESP8266_and_ESP32_Oled_Driver_for_SSD1306_display\src\SSD1306Wire.h changed to Wire.setClock(1000000); from 700K - seems to be the limit. May be faster ssd1306 libs or wire libs? BUt that said SSD1306&esp32 so I used it and it works to about 50 Hz updates.

Used one of the samples there - added button reads to force one of the list of tests to run [0,1,2,3] and read light sensor and print a 'Lsen #' value between each 3 second stest display period. No analogWrite() or tone() to test the speaker pin #25 so it just prints a multiple of 200 that was to be the tone/freq. In room here the sensor reads 0 unless a lihgt is pointed at it. It toggles the blue PLAY LED #4 between each test

Not sure of the acceleromter for now ?

FIRST NOTES:
============
TinyPICO on Arduino with the ESP32 PICO seems decent so far!  I made this initial github entry and first sketch SPIFFS_time as a combo of the sample from UnexpectedMaker and SPIFFS_test and SPIFFS_time

TinyPICO ships with what seems to be : Spiffs Space Total=1374476  , but it needs to be Formatted for first use that the _test sketch did and integrated that into the _time that connects to WiFi and find ntp server to get time. Then it does some sample SPIFFS I/O - shows the time and starts the DotStar cycling and then monitors the PICO temp

Nothing special but it works to show Arduino is working to show the TinyPICO is working. Except I find the a WiFi sketch uploaded when WiFi is running takes second upload or power cycle - Seon says lots of WiFI fixes coming in Arduino 1.0.3 update so until that might offer a change - best to just know this. On first use the NTP time seems to come up as base time as well - maybe the first use of the WiFi stack gets relinked on subsequent use.

Using the ESP32 Pico Kit won't have access to PSRAM until the new update for the TinyPICO board - but them Seon says: "To use psram in Arduino, you need to use ps_malloc() instead of malloc()"
I've powered 4 TinyPICO's and they report various internal temps running above sketch from 108 °F to 180 °F - but all seem to be running about 120f  - the one that was measuring 128f last night is now showing 110°F - maybe because the Serial chip is not active?

Using Windows 10 here: One more thing one machine had good drivers for CP2104 - but desktop identified but not installed the driver - so manually grabbed it from :: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers(edited)
