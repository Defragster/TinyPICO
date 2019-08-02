# TinyPICO
TinyPICO - ESP32 PICO
---------------------
Getting Started: https://www.tinypico.com/gettingstarted

Hardware: https://www.crowdsupply.com/unexpected-maker/tinypico

Github: https://github.com/tinypico/tinypico-arduino : has #include <TinyPICO.h>


SPIFFS_time.ino
-----------------
First sample of combined samples - connects to WiFi {set SSID/PW } gets ntp {adjust timezone}, tests SPIFFS, shows space, prints TEMP/WiFi state each 10 secs and does TinyPICO RGB color change.


FIRST NOTES:
============
TinyPICO on Arduino with the ESP32 PICO seems decent so far!  I made this initial github entry and first sketch SPIFFS_time as a combo of the sample from UnexpectedMaker and SPIFFS_test and SPIFFS_time

TinyPICO ships with what seems to be : Spiffs Space Total=1374476  , but it needs to be Formatted for first use that the _test sketch did and integrated that into the _time that connects to WiFi and find ntp server to get time. Then it does some sample SPIFFS I/O - shows the time and starts the DotStar cycling and then monitors the PICO temp

Nothing special but it works to show Arduino is working to show the TinyPICO is working. Except I find the a WiFi sketch uploaded when WiFi is running takes second upload or power cycle - Seon says lots of WiFI fixes coming in Arduino 1.0.3 update so until that might offer a change - best to just know this. On first use the NTP time seems to come up as base time as well - maybe the first use of the WiFi stack gets relinked on subsequent use.

Using the ESP32 Pico Kit won't have access to PSRAM until the new update for the TinyPICO board - but them Seon says: "To use psram in Arduino, you need to use ps_malloc() instead of malloc()"
I've powered 4 TinyPICO's and they report various internal temps running above sketch from 108 °F to 180 °F - but all seem to be running about 120f  - the one that was measuring 128f last night is now showing 110°F - maybe because the Serial chip is not active?

Using Windows 10 here: One more thing one machine had good drivers for CP2104 - but desktop identified but not installed the driver - so manually grabbed it from :: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers(edited)
