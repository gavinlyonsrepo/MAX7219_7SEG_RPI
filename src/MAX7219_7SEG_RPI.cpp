/*!
	@file MAX7219_7SEG_RPI.cpp
	@author Gavin Lyons
	@brief library source file to drive MAX7219 displays 
	Project Name: MAX7219_7SEG_RPI
	@note  See URL for full details. https://github.com/gavinlyonsrepo/MAX7219_7SEG_RPI
*/
#include "MAX7219_7SEG_RPI.hpp"

// Public methods

/*!
	@brief Constructor for class MAX7219_SS_RPI software SPI
	@param clock CLk pin
	@param chipSelect CS pin
	@param data DIO pin 
	@note overloaded see MAX7219_SS_RPI(uint32_t KiloHertz, uint8_t SPICE_Pin)
*/
MAX7219_SS_RPI::MAX7219_SS_RPI(uint8_t clock, uint8_t chipSelect , uint8_t data)
{
	_MAX7219_CLK_IO = clock;
	_MAX7219_CS_IO  = chipSelect;
	_MAX7219_DIN_IO = data;
	_HardwareSPI = false;
}

/*!
	@brief Constructor for class MAX7219_SS_RPI hardware SPI
	@param kiloHertz SPI bus speed in kilohetrz
	@param SPICEX_PIN Which SPICX pin to use 1 or 0 
	@note overloaded see MAX7219_SS_RPI(uint8_t clock, uint8_t chipSelect , uint8_t data)
*/
MAX7219_SS_RPI::MAX7219_SS_RPI(uint32_t kiloHertz, uint8_t SPICEX_PIN)
{
	_KiloHertz = kiloHertz;
	_SPICEX_CS_IO = SPICEX_PIN;
	_HardwareSPI = true;
}

/*!
	@brief End display operations, called at end of program before closing bcm2835 library.
*/
void MAX7219_SS_RPI::DisplayEndOperations(void)
{
	if (_HardwareSPI == false)
	{
		MAX7219_CS_SetLow;
		MAX7219_CLK_SetLow;
		MAX7219_DIN_SetLow;
	}else
	{
		bcm2835_spi_end();
	}
}

/*!
	@brief get value of _HardwareSPI , true hardware SPI on , false off.
*/
bool MAX7219_SS_RPI::GetHardwareSPI(void)
{return _HardwareSPI;}


/*!
	@brief Init the display
	@param numDigits scan limit set to 8 normally , advanced use only 
	@param decodeMode Always 0x00 or "No decode for digits 7–0" for this library.
*/
void MAX7219_SS_RPI::InitDisplay(uint8_t numDigits, uint8_t decodeMode)
{

	if (_HardwareSPI == false )
	{
		MAX7219_CS_SetDigitalOutput;
		MAX7219_CLK_SetDigitalOutput;
		MAX7219_DIN_SetDigitalOutput;
		MAX7219_CS_SetHigh;
	}else
	{
		bcm2835_spi_begin();
		bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
		bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); 
		
		// SPI bus speed
		if (_KiloHertz > 0)
			bcm2835_spi_setClockDivider(bcm2835_aux_spi_CalcClockDivider(_KiloHertz));
		else // default, BCM2835_SPI_CLOCK_DIVIDER_64 3.90MHz Rpi2, 6.250MHz RPI3
			bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); 
		
		// Chip enable pin select
		if (_SPICEX_CS_IO == 0)
		{
			bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
			bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
		}else if (_SPICEX_CS_IO == 1)
		{
			bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
			bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
		}
	}
	
	MAX7219_MilliSecondDelay(50); // small init delay before commencing transmissions
	
	_NoDigits = numDigits;
	SetScanLimit(numDigits-1);
	SetDecodeMode(decodeMode);
	ShutdownMode(false);
	DisplayTestMode(false);
	ClearDisplay();
	SetBrightness(IntensityDefault);
}


/*!
	@brief Clear the display
*/
void MAX7219_SS_RPI::ClearDisplay(void)
{
	for(uint8_t digit=0; digit<_NoDigits ; digit++)
	{
		WriteDisplay(digit+1, 0x00);
	}
}


/*!
	@brief Displays a character on display
	@param digit The digit to display character in, 7-0 ,7 = LHS 0 =RHS
	@param character  The ASCII character to display
	@param decimalPoint Is the decimal point(dp) to be set or not.
*/
void MAX7219_SS_RPI::DisplayChar(uint8_t digit, uint8_t character , DecimalPoint_e decimalPoint)
{
	WriteDisplay(digit+1,ASCIIFetch(character , decimalPoint));
}

/*!
	@brief Set a seven segment LED ON 
	@param digit The digit to set segment in, 7-0 ,7 = LHS 0 =RHS
	@param segment The segment of seven segment to set dpabcdefg
*/
void MAX7219_SS_RPI::SetSegment(uint8_t digit, uint8_t segment)
{
	WriteDisplay(digit+1, segment);
}

/*!
	@brief Displays a text string on display
	@param text pointer to character array containg text string
	@param TextAlignment  left or right alignment or leading zeros
	@note This method is overloaded, see also DisplayText(char *)
*/
void MAX7219_SS_RPI::DisplayText(char *text, TextAlignment_e TextAlignment){

	char character;
	char pos =0;
	
	// We need the length of the string - no of decimal points set
	uint8_t LengthOfStr = (_NoDigits);
	LengthOfStr=strlen(text);
	for(uint8_t index =0; text[index]; index++)
	{
		if(text[index] == '.') LengthOfStr--; // decrement string for dp's
	}
	if (LengthOfStr > (_NoDigits)) LengthOfStr = (_NoDigits);
	
	while ((character = (*text++)) && pos < _NoDigits)
	{
		if (*text == '.' && character != '.')
		{
			switch (TextAlignment) // Display a character with dp set 
			{
				case AlignLeft  : DisplayChar((_NoDigits-1)- pos ,character, DecPointOn); break;
				case AlignRight : DisplayChar((LengthOfStr-1)- pos ,character, DecPointOn); break;
				case AlignRightZeros: return; break;
			}
			pos++;
			text++;
		}  else
		{
			switch (TextAlignment) // Display a character without dp set 
			{
				case AlignLeft  : DisplayChar((_NoDigits-1) -pos, character, DecPointOff); break;
				case AlignRight : DisplayChar((LengthOfStr-1) - pos, character, DecPointOff); break;
				case AlignRightZeros : return; break;
			}
			pos++;
		}
	}
}


/*!
	@brief Displays a text string on display
	@param text  pointer to character array containg text string
	@note This method is overloaded, see also DisplayText(char *, TextAlignment_e )
*/
void MAX7219_SS_RPI::DisplayText(char *text){

	char character;
	char pos = _NoDigits-1;

	while ((character = (*text++)) && pos < _NoDigits)
	{
		if (*text == '.' && character != '.')
		{
			DisplayChar(pos  ,character, DecPointOn);
			pos--;
			text++;
		}  else
		{
			DisplayChar(pos  ,character, DecPointOff);
			pos--;
		}
	}
}

/*!
	@brief sets the brighttness of display
	@param brightness rang 0x00 to 0x0F , 0x00 being least bright.
*/
void MAX7219_SS_RPI::SetBrightness(uint8_t brightness)
{
	brightness &= IntensityMax;
	WriteDisplay(MAX7219_REG_Intensity, brightness);
}


/*!
	@brief Turn on and off the Shutdown Mode
	@param OnOff true = Shutdown mode on , false shutdown mode off
	@note power saving mode 
*/
void MAX7219_SS_RPI::ShutdownMode(bool OnOff)
{
	OnOff ? WriteDisplay(MAX7219_REG_ShutDown, 0) : WriteDisplay(MAX7219_REG_ShutDown, 1);
}


/*!
	@brief Turn on and off the Display Test Mode
	@param OnOff true = display test mode on , false display Test Mode off 
	@note Display-test mode turns all LEDs on
*/
void MAX7219_SS_RPI:: DisplayTestMode(bool OnOff)
{
	OnOff ? WriteDisplay(MAX7219_REG_DisplayTest, 1) : WriteDisplay(MAX7219_REG_DisplayTest, 0);
}


/*!
	@brief Set the communication delay value
	@param commDelay Set the communication delay value uS software SPI
*/
void MAX7219_SS_RPI::SetCommDelay(uint16_t commDelay)
{
	_CommDelay = commDelay;
}

/*!
	@brief Get the communication delay value
	@return Get the communication delay value uS Software SPi
*/
uint16_t  MAX7219_SS_RPI::GetCommDelay(void)
{
	return _CommDelay;
}


/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32
	@param TextAlignment enum text alignment, left or right alignment or leading zeros
*/
void  MAX7219_SS_RPI::DisplayIntNum(unsigned long number, TextAlignment_e TextAlignment)
{
	char values[_NoDigits+1];
	char TextDisplay[6] = "%";
	char TextRight[4] = "8ld";
	char TextLeft[3] = "ld";
	char TextLeadZero[5] = "08ld";

	switch(TextAlignment)
	{
		case AlignRight:
			strcat(TextDisplay ,TextRight); // %8ld
		break;
		case AlignLeft:
			strcat(TextDisplay ,TextLeft);  // %ld
		break;
		case AlignRightZeros:
			strcat(TextDisplay ,TextLeadZero);  // %08ld
		break;
	}
	snprintf(values, _NoDigits+1, TextDisplay, number);
	DisplayText(values);
}


/*!
	@brief Display an integer in a nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param TextAlignment  left or right alignment or leading zeros
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble.
*/
void MAX7219_SS_RPI::DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, TextAlignment_e TextAlignment)
{
	char valuesUpper[_NoDigits+ 1];
	char valuesLower[_NoDigits/2 + 1];
	char TextDisplay[5] = "%";
	char TextRight[3] = "4d";
	char TextLeft[4] = "-4d";
	char TextLeadZero[4] = "04d";

	switch(TextAlignment)
	{
		case AlignRight: strcat(TextDisplay ,TextRight); break; // %4d
		case AlignLeft: strcat(TextDisplay ,TextLeft); break;  // %-4d
		case AlignRightZeros: strcat(TextDisplay ,TextLeadZero); break; // %04d
	}

	snprintf(valuesUpper, _NoDigits/2 + 1, TextDisplay, numberUpper);
	snprintf(valuesLower, _NoDigits/2 + 1, TextDisplay, numberLower);
	strcat(valuesUpper ,valuesLower);
	
	DisplayText(valuesUpper);
}



// Private methods

 /*!
	@brief Shifts out a uint8_t of data on to the MAX7219 SPI-like bus
	@param value The uint8_t of data to shift out
	@note _CommDelay microsecond delay may have to be adjusted depending on processor
*/
void MAX7219_SS_RPI::HighFreqshiftOut(uint8_t value)
{

	for (uint8_t bit = 0; bit < 8; bit++)
	{
		!!(value & (1 << (7 - bit))) ? MAX7219_DIN_SetHigh: MAX7219_DIN_SetLow; // MSBFIRST
		MAX7219_CLK_SetHigh;
		MAX7219_MicroSecondDelay(_CommDelay);
		MAX7219_CLK_SetLow;
		MAX7219_MicroSecondDelay(_CommDelay);
	}
}


/*!
	@brief Fetch's the seven segment code for a given ASCII code from the font
	@param character The ASCII character to  lookup
	@param decimalPoint Is the decimal point(dp) to be set or not.
	@return The seven segment representation of the ASCII character in a byte dpabcdefg
*/
uint8_t MAX7219_SS_RPI::ASCIIFetch(uint8_t character, DecimalPoint_e decimalPoint)
{
	if (character<=31 || character>=123) {return 0;} // check ASCII font bounds

	const uint8_t  AsciiOffset = 0x20; // The font starts at ASCII 0x20 . space
	uint8_t returnCharValue =0;

	switch (decimalPoint)
	{
		case DecPointOn :
			returnCharValue  = pSevenSegASCIIFont[character - AsciiOffset];
			returnCharValue |= (1<<7);
			return (returnCharValue);
		break;
		case DecPointOff :
			return pSevenSegASCIIFont[character - AsciiOffset];
		break;
	}

	return 0;
}

/*!
	@brief Write to the MAX7219 display register
	@param RegisterCode the register to write to
	@param data The data byte to send to register
*/
void MAX7219_SS_RPI::WriteDisplay( uint8_t RegisterCode, uint8_t data) 
{
	if (_HardwareSPI == false)
	{
		MAX7219_CS_SetLow;
		HighFreqshiftOut(RegisterCode);
		HighFreqshiftOut(data);
		MAX7219_CS_SetHigh;
	}else
	{
		uint8_t TransmitBuffer[2];
		TransmitBuffer[0] = RegisterCode;
		TransmitBuffer[1] = data;
		bcm2835_spi_writenb((char*)TransmitBuffer,sizeof(TransmitBuffer));
	}
}

/*!
	@brief Set the decode mode of the  MAX7219 decode mode register
	@param mode Always 0x00 or "No decode for digits 7–0" for this library.
*/
void MAX7219_SS_RPI::SetDecodeMode(uint8_t mode)
{
	WriteDisplay(MAX7219_REG_DecodeMode , mode);
}

/*!
	@brief Set the decode mode of the  MAX7219 decode mode register
	@param numDigits Usually set to 7(digit 8) The scan-limit register sets how many digits are displayed,
	from 1 to 8.
	@note Advanced users only , read datasheet
*/
void MAX7219_SS_RPI::SetScanLimit(uint8_t numDigits)
{
	WriteDisplay(MAX7219_REG_ScanLimit, numDigits);
}


// == EOF ==
