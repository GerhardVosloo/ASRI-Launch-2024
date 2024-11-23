# CT-UKZN-Suborbital-Rideshare
![image](https://user-images.githubusercontent.com/107643197/221375419-d97fc3f7-f874-4c50-b7d7-c1082380f069.png)

Code for 2024 ASRI Pheonix 1E Suborbital rocket launch

*Made for the domino4 / Xinabox chipsets

## Purpose
To record data such as:
 - Barometric pressure
 - Temperature
 - V0C2 readings
 - Etc

during a short suborbital rocket journey.

The payload contains 2 ESP32 dev boards each taking different data points from sensors connected to an I2C line and record the data onto
a CSV data file stored on an onboard MicroSD card.
## Design
| Slice | Air Quality | Accelerometer | Weather | Barometer | Code | Contributor
| -- | -- | -- | -- | -- | -- | -- |
| 1 |✅|✅|❌|✅| ... | ... |
| 2 | | | | | | |
| 3 | | | | | | |
| 4 | | | | | | |

## Libraries
AHTxx-Main
[IGA](https://github.com/domino4com/IGA) Air Quality Sensor
[IIA](https://github.com/domino4com/IIA) Accelerometer
[IWA](https://github.com/domino4com/IWA) Weather Sensor
[IWB](https://github.com/domino4com/IWB) Barometer
