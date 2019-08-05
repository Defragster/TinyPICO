# RSA Signature Speed - Simple CPU Performance Benchmark

Measures the time required to compute a digital signature (SHA256 + RSA2048) of a string.
--------
This code made ESP32 RTOS TASK specific to test Dual Cores. Generic code from here: https://github.com/PaulStoffregen/CoreMark also works on default Arduino single core.

As written TinyPICO shows .510 seconds per core on either or both cores 

| Board                  | Seconds |
| ---------------------- | :-----: |
| Teensy 4.0             |  0.085  |
| Teensy 3.6 @256 Mhz ** |  0.333  |
| Teensy 3.6 @180 Mhz    |  0.474  |
| Sparkfun ESP32 Thing   |  0.518  |
| TinyPICO ESP32 2 core  |  0.510 & .510  |
| Metro M4 Grand Central |  0.840  |
| Teensy 3.5             |  0.909  |
| Teensy 3.2             |  1.325  |
| Arduino Due            |  1.901  |
| Arduino Zero           |  9.638  |

(smaller numbers are better)
(numbers at default IDE settings - not OC'd - except ** )
