# Changelog

* version 1.2.0 May 2023
	* first release
* version 1.3.0 Dec 2023
	* Changed Makefile CCFLAGS so it complies on 64 bit system without errors.
	* Changed "InitDisplay" function so it returns false if SPi-begin fails This catches user error of not running as root.
* version 1.4.0 Nov 2024 
	* Minor update,  added function MAX7219SPIHWSettings() to allow multiple devices on hardware SPI bus, see readme for
	details
