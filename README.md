# Led_Driver
Device Driver for Raspberry Pi 3 (Raspbian)

# Description
A program that converts hexadecimal numbers to binary numbers

# DEMO

# Development environment
- RaspberryPi 3
  - No LSB modules are available.
  - Distributor ID: Raspbian
  - Description:    Raspbian GNU/Linux 9.4 (stretch)
  - Release:        9.4
  - Codename:       stretch

# Installation
## step1
### connect
GND----Resistor----GPIO <br>
I used GPIO4, 18, 17, 27, 22, 23, 24, 25

## step2
Download this repository.
```
$ git clone https://github.com/AtsuyaKoike/Led_Driver
```
## step3
Move derectory
```
$ cd Led_Driver/
```
## step4
- Running the Shell
  - Start -> preprocessing.sh
  - finish -> post_processing.sh
```
$ bash preprocessing.sh
$ bash post_processing.sh
```

# License
This repository is licensed under the GPLv3 license, see LICENSE.

