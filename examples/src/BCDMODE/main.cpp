/*!
	@file MAX7219_7SEG_RPI/examples/src/BCDMODE/main.cpp
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
	Carries out tests on BCD decode mode 3. BCD mode uses the built in font Code B
	this font has BCD code B (0-9, E, H, L, P, and -) 
	Hardware SPI , Project Name: MAX7219_7SEG_RPI
	

	@test
		Test 30 BCD character
		Test 31 BCD string
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
	
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeBCDThree);
	myMAX.ClearDisplay();
	
	// Test BCD Char
	myMAX.DisplayBCDChar(7, myMAX.CodeBFontH); // Digit 7 LHS of display
	myMAX.DisplayBCDChar(6, myMAX.CodeBFontE);
	myMAX.DisplayBCDChar(5, myMAX.CodeBFontL);
	myMAX.DisplayBCDChar(4, myMAX.CodeBFontP);
	myMAX.DisplayBCDChar(3, myMAX.CodeBFontSpace);
	myMAX.DisplayBCDChar(2, myMAX.CodeBFontFour);
	myMAX.DisplayBCDChar(1, myMAX.CodeBFontDash);
	myMAX.DisplayBCDChar(0, myMAX.CodeBFontOne); // Digit 0 RHS
	MAX7219_MilliSecondDelay(5000);
	myMAX.ClearDisplay();
	
	// Test BCD string 
	char teststring1[] = "PH  3-14";
	myMAX.DisplayBCDText(teststring1);
	MAX7219_MilliSecondDelay(5000);
	myMAX.ClearDisplay();
	
	myMAX.DisplayEndOperations();
	
	// Close the bcm2835 library
	bcm2835_close(); 
	printf("Test End\r\n");
	return 0;
} 
// End of main

// EOF
