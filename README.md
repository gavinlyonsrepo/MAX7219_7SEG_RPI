[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)


# MAX7219_7SEG_RPI

![ max ](https://github.com/gavinlyonsrepo/MAX7219_7SEG_RPI/blob/main/extra/image/max.jpg)

## Table of contents

  * [Overview](#overview)
  * [Installation](#installation)
  * [Test](#test)
  * [Software](#software)
  * [Hardware](#hardware)
  * [Notes and Issues](#notes-and-issues)
	* [Cascaded Displays](#cascaded-displays)


## Overview

* Name: MAX7219_7SEG_RPI
* Author: Gavin Lyons.
* See also : [Combined Display library 'Display_Lib_RPI'](https://github.com/gavinlyonsrepo/Display_Lib_RPI)

* Description:

A Raspberry pi library to display data on a 8-digit MAX7219 seven segment module.
Dynamic install-able system level Raspberry Pi C++ library.
Hardware or software SPI, Shutdown mode, test mode and Brightness control supported.

* Development Tool chain.
	1. Raspberry PI 3 model b
	2. C++, g++ (Debian 12.2.0) 
	3. Raspbian , Debian 12 bookworm OS, , 64 bit.
	4. kernel : aarch64 Linux 6.1.0-rpi7-rpi-v8
	5. bcm2835 Library 1.73 (Dependency: used for SPI, GPIO control and delays)

## Installation

1. Install the dependency bcm2835 Library if not installed
	* The bcm2835 library is a dependency and provides delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

2. Download the MAX7219_7SEG_RPI library
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.

```sh
curl -sL https://github.com/gavinlyonsrepo/MAX7219_7SEG_RPI/archive/1.3.tar.gz | tar xz
```

3. Run "make" to run the makefile in base folder and then  "make install" to install library, it will be
    installed to usr/lib and usr/include by default.

```sh
cd MAX7219_7SEG_RPI-1.3
make
sudo make install
```

## Test

Wire up your Display.
Next step is to test LED display and the just installed library with an example file.

There are 6 examples files. The default example file is  "hello world".
To decide which one the makefile(In examples folder) builds simply edit "SRC" variable
at top of the makefile(In examples folder). 
in the "User SRC directory Option Section" at top of file.

| List No | Example file name  | Description | SPI type |
| ------ | ------ |   ------ | ----- |
| 1 | src/TESTS/main.cpp |  test sequence  | software |
| 2 | src/HELLOWORLD_HWSPI/main.cpp | Hello world | hardware |
| 3 | src/TESTS_HWSPI/main.cpp |  test sequence  | hardware |
| 4 | src/BCDMODE/main.cpp | Shows use of BCD built-in font  | hardware |
| 5 | src/CLOCK_DEMO/main.cpp |  Clock demo , Shows use of cascaded displays | hardware |
| 6 | src/CASCADE_DEMO/main.cpp | simple Demo showing use of cascaded displays | hardware |

Next enter the examples folder and run the makefile in THAT folder,
This makefile builds the examples file using the just installed library.
and creates a test exe file in "bin".

```sh
cd examples
make
make run
```

## Software

There are two makefiles:

    1. Root directory, builds and installs library at a system level.
    2. Example directory, builds example file using installed library to an executable.

## Hardware

For Software SPI Pick any GPIO you want.
For Hardware SPI the User must use fixed SPI pins SPIMOSI and SPISCLK, user can choice between SPICE0 and SPICE1 
for chip select. The Datasheet says it's a 10MHZ device, In hardware SPI user can pick SPI bus speed.
In software SPI user may need to increase or decrease CommDelay variable (uS Communication delay) depending on speed 
of CPU on system. User can adjust brightness from 0x00 to 0x0f by default it is 0x08. 0x0f being brightest
 
Connections to RPI:

| Pin no  | RPI SW SPI | RPI HW SPI  | Pin function |
| --- | --- | --- | --- |
| 1 | any GPIO output | SPISCLK |  CLK = Clock |
| 2 | any GPIO output | SPICE0 or SP1CE1 | CS = Chip select |
| 3 | any GPIO output | SPIMOSI |  DIN = Data in |


VCC 5V in theory but works at 3.3V in testing, albeit with a dimmer display.
Logic seems to work fine at 3.3V , It safe as it is all one way communications
From RPI to MAX.


## Notes and Issues

### Cascaded Displays

Support for Cascaded Displays added in Version 1.2 but untested as only one display available.
Casacded Displays are displays connected together. Din-> Dout and CS lines tied together.

