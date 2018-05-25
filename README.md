# WeatherRover

### Overview

WheaterRover is a project build on STM32 of remote controlled car with weather sensor, that uses bluetooth communication (Android applicaton) to control direction of movements and send weather data.

### Tools:
* Microcontroller STM32F407VG DISCOVERY
* Bluetooth BLE - HM-10 MLT-BT05
* Chassis 4WD Robot Car
* L298N motor driver
* Ultrasonic sensor HC-SR04
* DHT11
* SD card module
* Battery Samsung ICR 18650-22F

### Connections:
Bluetooth:
* VCC	->	VCC 
* GND	->	GND
* TX	->	PC11
* RX	->	PC10

l298n:
* IN1 -> PD13
* IN2 -> PD15
* IN3 -> PD14
* IN4 -> PD12

HC SR04
* TRIG -> PE4
* ECHO -> PE5

DHT11
* OUT -> PA1
* VCC -> 3V
* GND -> GND

### How to compile:
* System Workbench for STM32
* Android Studio

### Credits:
* **Adam Kurzawa** - [kuadam](https://github.com/kuadam)
* **Tomasz Łoński** - [lonskiTomasz](https://github.com/lonskiTomasz)
* **Sławomir Łyskawiński** - [SlavekL](https://github.com/SlavekL)

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
Supervisor: Tomasz Mańkowski
