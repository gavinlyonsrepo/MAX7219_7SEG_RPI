/*!
	@file MAX7219_7SEG_RPI/examples/src/HELLOWORLD/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
	Carries out most basic use case/test , "hello world" ~ helowrld
	Project Name: MAX7219_7SEG_RPI
	
	@test
		Test 0 Hello World
*/

// Libraries 
#include <bcm2835.h>
#include <stdio.h>
#include <MAX7219_7SEG_RPI.hpp> 

// GPIO I/O pins on the raspberry pi ,pick on any I/O you want.
#define  CLK 25  // clock GPIO, connected to clock line of module
#define  CS 24   // Chip Select GPIO, connected to CS line of module
#define  DIN 23  // data in GPIO, connected to DIN line of module

// Constructor object 
MAX7219_SS_RPI myMAX(CLK, CS ,DIN);

// Main loop
int main(int argc, char **argv) 
{

	// Init the bcm2835 library
	printf("Test Begin :: MAX7219_7SEG_RPI\r\n");
	if(!bcm2835_init()) {return -1;}
	
	// Hello world test on MAX7219
	char teststr1[] = "HElowrld";
	myMAX.InitDisplay(8, 0x00);
	myMAX.ClearDisplay();
	myMAX.DisplayText(teststr1, myMAX.AlignRight);
	MAX7219_MilliSecondDelay(5000);
	myMAX.ClearDisplay();
	
	// Close the bcm2835  ibrary
	bcm2835_close(); 
	printf("Test End\r\n");
	return 0;
} 
// End of main

// EOF
