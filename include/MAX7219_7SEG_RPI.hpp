/*!
	@file MAX7219_7SEG_RPI.hpp
	@author Gavin Lyons
	@brief library header file to drive MAX7219 displays 
	Project Name: MAX7219_7SEG_RPI
	@note  See URL for full details. https://github.com/gavinlyonsrepo/MAX7219_7SEG_RPI
*/

#pragma once

// Libraries
#include <bcm2835.h>
#include <cstring>
#include <cstdio> //snprintf
#include "MAX7219_7SEG_RPI_Font.hpp"

// GPIO abstraction
#define MAX7219_CS_SetHigh  bcm2835_gpio_write(_MAX7219_CS_IO, HIGH)
#define MAX7219_CS_SetLow   bcm2835_gpio_write(_MAX7219_CS_IO, LOW)
#define MAX7219_CLK_SetHigh bcm2835_gpio_write(_MAX7219_CLK_IO, HIGH)
#define MAX7219_CLK_SetLow  bcm2835_gpio_write(_MAX7219_CLK_IO, LOW)
#define MAX7219_DIN_SetHigh bcm2835_gpio_write(_MAX7219_DIN_IO, HIGH)
#define MAX7219_DIN_SetLow  bcm2835_gpio_write(_MAX7219_DIN_IO,LOW)

#define MAX7219_CS_SetDigitalOutput  bcm2835_gpio_fsel(_MAX7219_CS_IO, BCM2835_GPIO_FSEL_OUTP)
#define MAX7219_CLK_SetDigitalOutput bcm2835_gpio_fsel(_MAX7219_CLK_IO, BCM2835_GPIO_FSEL_OUTP)
#define MAX7219_DIN_SetDigitalOutput bcm2835_gpio_fsel(_MAX7219_DIN_IO, BCM2835_GPIO_FSEL_OUTP)

// Delay abstraction
#define MAX7219_MicroSecondDelay bcm2835_delayMicroseconds
#define MAX7219_MilliSecondDelay bcm2835_delay

/*!
	@brief  The main Class , used drive MAX7219 seven segment displays
*/
class MAX7219_SS_RPI
{
public:
	MAX7219_SS_RPI(uint8_t clock, uint8_t chipSelect ,uint8_t data);
	MAX7219_SS_RPI(uint32_t kiloHertz, uint8_t SPICEX_PIN);
	
	/*! Alignment of text on display */
	enum TextAlignment_e : uint8_t
	{
		AlignLeft       = 0,  /**< Align text to the left on  display */
		AlignRight      = 1,  /**< Align text to the right on  display */
		AlignRightZeros = 2   /**< Add leading zeros  to the text */
	};

	/*! Activate Decimal point segment */
	enum DecimalPoint_e : uint8_t
	{
		DecPointOff  = 0, /**< Decimal point segment off */
		DecPointOn   = 1  /**< Decimal point segment on */
	};

	/*! Set intensity/brightness of Display */
	enum Intensity_e : uint8_t
	{
		IntensityMin     = 0x00, /**< Minimum Intensity */
		IntensityDefault = 0x08, /**< Default Intensity */
		IntensityMax     = 0x0F  /**<  Maximum Intensity */
	};

	/*! Register opcodes of the MAZ7219 chip, Register Address Map */
	enum RegisterModes_e : uint8_t
	{
		MAX7219_REG_NOP          = 0x00, /**<  No operation */
		MAX7219_REG_DecodeMode   = 0x09, /**<  Decode-Mode Register */
		MAX7219_REG_Intensity    = 0x0A, /**<  Intensity Register, brightness of display */
		MAX7219_REG_ScanLimit    = 0x0B, /**<  Scan Limit,  The scan-limit register sets how many digits are displayed */
		MAX7219_REG_ShutDown     = 0x0C, /**<  When the MAX7219 is in shutdown mode, the scan oscillator is
												halted, all segment current sources are pulled to ground,
												and all digit drivers are pulled to V+, thereby blanking the
												display.  */
		MAX7219_REG_DisplayTest  = 0x0F  /**<  Display-test mode turns all LEDs on by
												overriding, but not altering, all controls and digit registers */
	};
	
	void InitDisplay(uint8_t numDigits, uint8_t decodeMode);
	void ClearDisplay(void);

	void DisplayChar(uint8_t digit, uint8_t value, DecimalPoint_e decimalPoint);
	void DisplayText(char *text, TextAlignment_e TextAlignment);
	void DisplayText(char *text);
	void SetSegment(uint8_t digit, uint8_t segment);
	void DisplayIntNum(unsigned long number, TextAlignment_e TextAlignment);
	void DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, TextAlignment_e TextAlignment);

	void SetBrightness(uint8_t brightness);
	void DisplayTestMode(bool OnOff);
	void ShutdownMode(bool OnOff);

	void SetCommDelay(uint16_t commDelay);
	uint16_t GetCommDelay(void);
	
	bool GetHardwareSPI(void);
	
	void DisplayEndOperations(void);
	
private:
	uint8_t _MAX7219_CS_IO;   /**<  GPIO connected to  CS on MAX7219  SW SPI only */
	uint8_t _MAX7219_DIN_IO;  /**<  GPIO connected to DIO on MAX7219  SW SPI only */
	uint8_t _MAX7219_CLK_IO;  /**<  GPIO connected to CLK on MAX7219  SW SPI only */

	uint16_t _CommDelay = 0; /**<  uS delay used in communications SW SPI, User adjust */
	uint8_t _NoDigits   = 8; /**<  Number of digits in display */

	uint32_t _KiloHertz = 5000;   /**< Spi freq in kiloHertz , MAX 125 Mhz MIN 30Khz */
	uint8_t  _SPICEX_CS_IO = 0;  /**< value = X , which SPI_CE pin to use, X = 1 or 0 */
	bool _HardwareSPI = false;  /**< Is the Hardware SPI on , true yes , false SW SPI*/
	
	void HighFreqshiftOut(uint8_t value);
	void WriteDisplay(uint8_t RegisterCode, uint8_t data);
	uint8_t ASCIIFetch(uint8_t character,DecimalPoint_e decimalPoint);
	void SetDecodeMode(uint8_t mode);
	void SetScanLimit(uint8_t numDigits);
};

