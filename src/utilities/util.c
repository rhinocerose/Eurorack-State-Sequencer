#include "LPC804.h"
#include "board.h"
//#include "core_cm0plus.h"
#include <stdio.h>
#include "util.h"
#include "DAC8568.h"
#include "spi_config.h"
#include "fsl_SSD1306.h"


void Board_Init(void)
{
	Board_ClkInit();
	Board_GPIOInit();
	SPI_Init();
	OLED_Init();
	Timers_Init();
}

void Board_ClkInit(void)
{
    SYSCON->MAINCLKSEL 		= (0x0<<SYSCON_MAINCLKSEL_SEL_SHIFT);
    SYSCON->MAINCLKUEN 		&= (0x1);
    SYSCON->MAINCLKUEN 		|= (0x1);
    BOARD_BootClockFRO30M();
}

void Board_GPIOInit(void)
{
	SYSCON->SYSAHBCLKCTRL0 |= (SYSCON_SYSAHBCLKCTRL0_GPIO0_MASK);
	SYSCON->PRESETCTRL0	  &= ~(SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK  );
	SYSCON->PRESETCTRL0	   |= (SYSCON_PRESETCTRL0_GPIO0_RST_N_MASK );
}

void Board_IRQInit(int channel, uint32_t pin, uint8_t edge)
{
	__disable_irq();
	NVIC_DisableIRQ(PIN_INT0_IRQn);

	SYSCON->PINTSEL[channel] = pin;

	switch (edge)
	{
		case irq_edge_Rising:
			PINT->ISEL			= 0x00;
			PINT->SIENR			= 0b00000001;// bit 0 is 1: enable channel 0 IRQ for rising edge
			PINT->CIENF			= 0b00000001;// bit 0 is 1: disable channel 0 IRQ for falling edge
			break;
		case irq_edge_Falling:
			PINT->ISEL			= 0x00;
			PINT->CIENR			= 0b00000001;// bit 0 is 1: disable channel 0 IRQ for rising edge
			PINT->SIENF			= 0b00000001;// bit 0 is 1: enable channel 0 IRQ for falling edge
			break;
		case irq_edge_Both:
			PINT->ISEL			= 0x00;
			PINT->SIENR			= 0b00000001;// bit 0 is 1: enable channel 0 IRQ for rising edge
			PINT->SIENF			= 0b00000001;// bit 0 is 1: enable channel 0 IRQ for falling edge
			break;
		case irq_Level:
			PINT->ISEL			= 0x00;
			break;
	}

	// Remove any pending or left-over interrupt flags
	PINT->IST			= 0xFF;

	NVIC_EnableIRQ(PIN_INT0_IRQn);// GPIO interrupt
	__enable_irq();
}

void PIN_INT0_IRQHandler(void)
{
	//PINT->IST			= 0xFF;
	// was an IRQ requested for Channel 0 of GPIO INT?
	if(PINT->IST& (1<<0))
	{
		PINT->IST		= (1<<0);	// Toggle the LED
		GPIO->NOT[0] 	= (1UL<<12);
		printf("Encoder moved");
	}
	else
	{
		asm("NOP");  // Place a breakpt here if debugging.
	}
	return;
}

void GPIO_PinConfig(uint32_t pin, uint8_t direction)
{
	switch (direction)
	{
		case gpio_Output_ON:
			GPIO->CLR[0] 		= (1UL<<pin);
			GPIO->DIRSET[0] 	= (1UL<<pin);
			break;
		case gpio_Output_OFF:
			GPIO->CLR[0] 		= (1UL<<pin);
			GPIO->DIRSET[0] 	= (1UL<<pin);
			GPIO->SET[0]		= (1UL<<pin);
			break;
		case gpio_Input:
			GPIO->DIRCLR[0] 	= (1UL<<pin);
			break;
	}
}

uint32_t GPIO_GetVal(uint32_t pin)
{
	return GPIO->B[0][pin];
}

void GPIO_OutputHigh(uint32_t pin)
{
    GPIO->CLR[0] 		= (1UL<<pin);
}

void GPIO_OutputLow(uint32_t pin)
{
    GPIO->SET[0] 		= (1UL<<pin);
}

void Delay_us(uint32_t microseconds)
{
	for (int i=0; i < (microseconds*15); i++)
	{
		asm("NOP");
	}
}

void Delay_s(uint32_t seconds)
{
    for (int i=0; i < (seconds*15000000); i++)
    {
    	asm("NOP");
    }
}
