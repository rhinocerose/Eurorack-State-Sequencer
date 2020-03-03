#include "spi_config.h"

void SPI_Init(void)
{
	SPI_PinInit(SPI_PIN_SCK, SPI_PIN_MOSI, SPI_PIN_CS0, SPI_PIN_CS1);

	spi_master_config_t masterConfig = {0};
    uint32_t 			srcFreq = 0U;

    CLOCK_Select		(kSPI0_Clk_From_MainClk);

    SPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps 	= SPI_MASTER_BAUDRATE;
    masterConfig.sselNumber 	= SPI_MASTER_SSEL;
    masterConfig.clockPhase 	= SPI_CLK_PHASE_1_EDGE;
    srcFreq 					= SPI_MASTER_CLK_FREQ;

    SPI_MasterInit(SPI_MASTER, &masterConfig, srcFreq);
}

void SPI_MasterInit2(void)
{
	SPI_PinInit(SPI_PIN_SCK, SPI_PIN_MOSI, SPI_PIN_CS0, SPI_PIN_CS1);

	SYSCON->SPI0CLKSEL = 0x1;

	// Get main_clk frequency
	SystemCoreClockUpdate();

	// Configure the SPI master's clock divider (value written to DIV divides by value+1)
	SPI0->DIV = (kFRG0_Clk_From_Fro/SPI_BAUD_RATE) - 1;

	// Configure the CFG register:
	// Enable=true, master, no LSB first, CPHA=0, CPOL=0, no loop-back, SSEL active low
	SPI0->CFG = SPI_CFG_ENABLE(1) | SPI_CFG_MASTER(1);

	// Configure the SPI delay register (DLY)
	// Pre-delay = 0 clocks, post-delay = 0 clocks, frame-delay = 0 clocks, transfer-delay = 0 clocks
	SPI0->DLY = 0x0000;

	// Configure the SPI control register
	// Master: End-of-frame true, End-of-transfer true, RXIGNORE true, LEN 8 bits.
	SPI0->TXCTL = SPI_TXCTL_EOF(1) | SPI_TXCTL_EOT(1) | SPI_TXCTL_RXIGNORE(1) | SPI_TXCTL_LEN(8);
}

void SPI_TxTransfer(SPI_Type *spi_port, uint8_t data)
{
	// Wait for TXRDY on SPI0
	while (!(spi_port->STAT & SPI_STAT_TXRDY(1)));

	spi_port->TXDAT = data;           // Transmit data

}

uint8_t SPI_Transfer(SPI_Type *spi_port, uint8_t data)
{
  while ( (spi_port->STAT & SPI_STAT_TXRDY(1)) == 0 );
  spi_port->TXDATCTL = SPI_TXDATCTL_LEN(8-1) | SPI_TXDATCTL_EOT(1) | data;
  while ( (spi_port->STAT & SPI_STAT_RXRDY(1)) == 0 );
  return spi_port->RXDAT;
}

void SPI_PinInit(uint16_t SCK, uint16_t MOSI, uint16_t SSEL0, uint16_t SSEL1)
{
	// Initialize SWM and SPIO0
	SYSCON->SYSAHBCLKCTRL0 			|= ( SYSCON_PRESETCTRL0_SWM_RST_N_MASK
									   | SYSCON_SYSAHBCLKCTRL0_SPI0_MASK  );

	// Reset SWM and SPIO0 peripherals
	SYSCON->PRESETCTRL0	  			&= ~( SYSCON_PRESETCTRL0_SWM_RST_N_MASK
										| SYSCON_PRESETCTRL0_SPI0_RST_N_MASK);
	SYSCON->PRESETCTRL0	  			|= ( SYSCON_PRESETCTRL0_SWM_RST_N_MASK
									   | SYSCON_PRESETCTRL0_SPI0_RST_N_MASK );

	// Assign SPI pins
	SWM0->PINASSIGN.PINASSIGN2	  	&= ~(SWM_PINASSIGN2_SPI0_SCK_IO(0xFF));
	SWM0->PINASSIGN.PINASSIGN2	  	|= (SWM_PINASSIGN2_SPI0_SCK_IO(SCK));

	SWM0->PINASSIGN.PINASSIGN2	  	&= ~(SWM_PINASSIGN2_SPI0_MOSI_IO(0xFF));
	SWM0->PINASSIGN.PINASSIGN2	  	|= (SWM_PINASSIGN2_SPI0_MOSI_IO(MOSI));

	SWM0->PINASSIGN.PINASSIGN2	    &= ~(SWM_PINASSIGN2_SPI0_SSEL0_IO(0xFF));
	SWM0->PINASSIGN.PINASSIGN2	    |= (SWM_PINASSIGN2_SPI0_SSEL0_IO(SSEL0));

	SWM0->PINASSIGN.PINASSIGN3	    &= ~(SWM_PINASSIGN3_SPI0_SSEL1_IO(0xFF));
	SWM0->PINASSIGN.PINASSIGN3	    |= (SWM_PINASSIGN3_SPI0_SSEL1_IO(SSEL1));

	// Turn off clock to SWM
	SYSCON->SYSAHBCLKCTRL0 		    &= ~(SYSCON_PRESETCTRL0_SWM_RST_N_MASK);
}
