/*!
	@file MAX7219_7SEG_RPI/examples/src/HELLOWORLD_HWSPI/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
	Carries out most basic use case/test , "hello world" ~ helowrld
	Hardware SPI
	
	Project Name: MAX7219_7SEG_RPI
	
	@test
		Test 0 Hello World
*/

// Libraries 
#include <bcm2835.h>
#include <stdio.h>
#include <MAX7219_7SEG_RPI.hpp> 

// Hardware SPI setup
uint32_t SPI_SCLK_FREQ =  5000; // HW Spi only , freq in kiloHertz , MAX 125 Mhz MIN 30Khz
uint8_t SPI_CEX_GPIO   =  0;     // HW Spi only which HW SPI chip enable pin to use,  0 or 1

// Constructor object 
MAX7219_SS_RPI myMAX(SPI_SCLK_FREQ, SPI_CEX_GPIO);

// Main loop
int main(int argc, char **argv) 
{

	// Init the bcm2835 library
	printf("Test Begin :: MAX7219_7SEG_RPI\r\n");
	if(!bcm2835_init()) {return -1;}
	
	// Hello world test on MAX7219
	char teststr1[] = "HElowrld";
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone);
	myMAX.ClearDisplay();
	myMAX.DisplayText(teststr1, myMAX.AlignRight);
	MAX7219_MilliSecondDelay(5000);
	myMAX.ClearDisplay();
	
	myMAX.DisplayEndOperations();
	// Close the bcm2835  library
	bcm2835_close(); 
	printf("Test End\r\n");
	return 0;
} 
// End of main

// EOF
