/*
 * spi_config.h
 *
 *  Created on: Feb. 7, 2020
 *      Author: ashar
 */

#include "fsl_spi.h"
#include "LPC804.h"
#include "util.h"
#include "fsl_SSD1306.h"
#include "DAC8568.h"


#define		SPI_CLK_PHASE_1_EDGE	(0x0U)
#define 	SPI_MASTER 				SPI0
#define 	CLK_SRC  				kCLOCK_MainClk
#define 	SPI_MASTER_CLK_FREQ 	CLOCK_GetFreq(CLK_SRC)
#define 	SPI_MASTER_BAUDRATE 	1000000UL
#define 	SPI_MASTER_SSEL 		kSPI_Ssel0Assert

#define		SPI_PIN_SCK		(10UL)
#define		SPI_PIN_MOSI	(16UL)
#define		SPI_PIN_CS0		(OLED_CS_PIN)
#define		SPI_PIN_CS1		(0xFF)
#define		SPI_BAUD_RATE	(100000)


void SPI_Init(void);

void SPI_MasterInit2(void);

void SPI_PinInit(uint16_t SCK, uint16_t MOSI, uint16_t SSEL0, uint16_t SSEL1);

uint8_t SPI_Transfer(SPI_Type *spi_port, uint8_t data);

void SPI_TxTransfer(SPI_Type *spi_port, uint8_t data);
