/*
  AD5668.cpp, version 1.1, written by Robert Hart, October 15, 2015

  This library implements both hardware-SPI and software SPI constructors.

  This is an Arduino library for the Analog Devices AD5668 16-bit, 8 Channel
  Digital-to-Analog converter chip. It should also be usable with the Texas
  Instruments DAC8568, a pin and command compatible part. However, to maintain
  full compatibility with the AD5668, this library does not impliment the
  "Flexible Mode" internal reference command write sequences of the DAC8568
  (0b1001).

  Portions of this code were based upon prior work by crx and Stephan Bergemann,
  specifically in the structure of the writeDAC and init functions, as well as
  the use of a command definition list. This library impliments all of the
  command functions of the chip, and allows for tying the ~LDAC pin to ground.
  When tying ~LDAC to ground, DO NOT use the toggleLDAC function. Also, use of the
  toggleDAC function is probably only needed in conjuction with command function
  writeChannel (chip command 0). For a full explanation of the various functions,
  refer to the readme file and the examples.

  The AD5668 library is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation, version 3 of the License, which should be included in this
  this distribution. If not, see <http://www.gnu.org/licenses/>.
*/

#include "DAC8568.h"
#include "fsl_spi.h"
#include "spi_config.h"
#include "util.h"

uint8_t 		channel,
				channelsN,
				clearCode;
uint16_t 		value;


void DAC_Init(void)
{
	GPIO_PinConfig			(DAC_RST_PIN, gpio_Output_ON);
	GPIO_PinConfig			(DAC_CS_PIN, gpio_Output_ON);

	DAC_reset();
	DAC_enableInternalRef();
	DAC_powerNormal(POWER_ON_DAC);
}

//void init() {
//	if (_hwSPI)
//	{
//		SPI.begin();
//		SPI.setBitOrder(MSBFIRST);
//	}
//	else
//	{
//		GPIO_OutputLow(DAC_PIN_MOSI);
//		GPIO_OutputHigh(DAC_PIN_SCK);
//	}
//	GPIO_OutputLow(DAC_CS_PIN);   // CS LOW
//	if (_ldacPin > 0)
//	{
//		GPIO_OutputHigh(DAC_PIN_LDAC);   // CS High
//	}
//	GPIO_OutputLow(DAC_PIN_CLR);   // CS LOW
//	delayMicroseconds(1);
//	GPIO_OutputHigh(DAC_PIN_CLR);   // CS High
//	delayMicroseconds(1);
//}

void writeDAC(uint8_t command, uint8_t address, uint16_t data, uint8_t function)
{
	GPIO_OutputHigh(DAC_CS_PIN);   // CS HIGH
	GPIO_OutputLow(DAC_CS_PIN);   // CS LOW

//	spi_transfer_t xfer = {0};
	uint8_t b1 = command;
	uint8_t b2 = address << 4 | data >> 12; //4 address bits and 4 MSBs of data
	uint8_t b3 = data >> 4; // middle 8 bits of data
	uint8_t b4 = (0xF0 & (data << 4) >> 8) | function;

//	xfer.txData = b1;
//	xfer.rxData = 0;
//	xfer.dataSize = sizeof(b1);
//	xfer.configFlags = kSPI_EndOfTransfer | kSPI_EndOfFrame |  kSPI_ReceiveIgnore;

//	SPI_MasterTransferBlocking(SPI0, &xfer);
//	xfer.txData = b2;
//	SPI_MasterTransferBlocking(SPI0, &xfer);
//	xfer.txData = b3;
//	SPI_MasterTransferBlocking(SPI0, &xfer);
//	xfer.txData = b4;
//	SPI_MasterTransferBlocking(SPI0, &xfer);

	SPI_Transfer(SPI0, b1);
	SPI_Transfer(SPI0, b2);
	SPI_Transfer(SPI0, b3);
	SPI_Transfer(SPI0, b4);

	GPIO_OutputHigh(DAC_CS_PIN);
}

void DAC_writeChannel(uint8_t channel, uint16_t value)
{
	writeDAC(WRITE_INPUT_REGISTER, channel, value, 15);
}

void DAC_updateChannel(uint8_t channel)
{
	writeDAC(UPDATE_OUTPUT_REGISTER, channel, 0, 15);
}

void DAC_writeChUpdateAll(uint8_t channel, uint16_t value)
{
	writeDAC(WRITE_INPUT_REGISTER_UPDATE_ALL, channel, value, 15);
}

void DAC_writeUpdateCh(uint8_t channel, uint16_t value)
{
	writeDAC(WRITE_INPUT_REGISTER_UPDATE_N, channel, value, 15);
}

void DAC_powerNormal(uint8_t channelsN)
{
	uint16_t modeChE_H = channelsN >> 4;
	uint8_t chA_D = 0x0F & channelsN;
	writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void DAC_powerDown1K(uint8_t channelsN)
{
	uint16_t modeChE_H = 0x10 | (channelsN >> 4);
	uint8_t chA_D = 0x0F & channelsN;
	writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void DAC_powerDown100K(uint8_t channelsN)
{
	uint16_t modeChE_H = 0x20 | (channelsN >> 4);
	uint8_t chA_D = 0x0F & channelsN;
	writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void DAC_powerDownTri(uint8_t channelsN)
{
	uint16_t modeChE_H = 0x30 | (channelsN >> 4);
	uint8_t chA_D = 0x0F & channelsN;
	writeDAC(POWER_DOWN_UP_DAC, 0, modeChE_H, chA_D);
}

void DAC_setClearCode(uint8_t clearCode)
{
	writeDAC(LOAD_CLEAR_CODE_REGISTER, 0, 0, clearCode);
}

void DAC_setSoftLDAC(uint8_t channelsN)
{
	uint16_t chE_H = channelsN >> 4;
	uint8_t chA_D = 0x0F & channelsN;
	writeDAC(LOAD_LDAC_REGISTER, 0, chE_H, chA_D);
}

void DAC_reset()
{
	writeDAC(RESET_POWER_ON, 0, 0, 0);
}

void DAC_enableInternalRef()
{
	writeDAC(SETUP_INTERNAL_REF, 0, 0, 1);
}

void DAC_disableInternalRef()
{
	writeDAC(SETUP_INTERNAL_REF, 0, 0, 0);
}

//void DAC_toggleLDAC() {
//  GPIO_OutputLow(DAC_LDAC_PIN);   // CS Low;
//  delayMicroseconds(1);
//  GPIO_OutputHigh(DAC_LDAC_PIN);   // CS High
//  delayMicroseconds(1);
//}
