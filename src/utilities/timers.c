#include "timers.h"

void Timers_Init(void)
{
	MRT_Enable();
	Systick_Enable();
	WKT_Enable();
}

void MRT_Enable(void)
{
	__disable_irq();
	NVIC_DisableIRQ(MRT0_IRQn);
	SYSCON->SYSAHBCLKCTRL0|= (SYSCON_SYSAHBCLKCTRL0_MRT_MASK);//
	SYSCON->PRESETCTRL0&= ~(SYSCON_PRESETCTRL0_MRT_RST_N_MASK);// reset MRT (bit=0)
	SYSCON->PRESETCTRL0|= (SYSCON_PRESETCTRL0_MRT_RST_N_MASK);// clear reset (bit=1)

	MRT0->CHANNEL[0].CTRL	= (MRT_ONESHOT << MRT_CHANNEL_CTRL_MODE_SHIFT
										| MRT_CHANNEL_CTRL_INTEN_MASK);
	MRT0->CHANNEL[1].CTRL	= (MRT_ONESHOT << MRT_CHANNEL_CTRL_MODE_SHIFT
										| MRT_CHANNEL_CTRL_INTEN_MASK);

	MRT0->CHANNEL[0].INTVAL = MRT_INIT_CH0 & ~(MRT_CHANNEL_INTVAL_LOAD_MASK);
	MRT0->CHANNEL[1].INTVAL	= MRT_INIT_CH1 & ~(MRT_CHANNEL_INTVAL_LOAD_MASK);
	NVIC_EnableIRQ(MRT0_IRQn);
	__enable_irq();
}

void Systick_Enable(void)
{
	__disable_irq();
	NVIC_DisableIRQ(SysTick_IRQn);
	SysTick_Config(SYSTICK_COUNTER);
	NVIC_EnableIRQ(SysTick_IRQn); // SysTick IRQs are on.
	__enable_irq();
}

void SysTick_Handler(void)
{
	GPIO->NOT[0] = (1UL<<LED1);
	return;
}

void WKT_Enable(void)
{
	__disable_irq();
	NVIC_DisableIRQ(WKT_IRQn);

	SYSCON->SYSAHBCLKCTRL0 	|= (SYSCON_SYSAHBCLKCTRL0_WKT_MASK);
	SYSCON->PDRUNCFG 		&= ~(SYSCON_PDRUNCFG_LPOSC_PD_MASK);
	SYSCON->LPOSCCLKEN 		|= (SYSCON_LPOSCCLKEN_WKT_MASK);
	SYSCON->PRESETCTRL0 	&= ~(SYSCON_PRESETCTRL0_WKT_RST_N_MASK); // Reset the WKT
	SYSCON->PRESETCTRL0 	|= (SYSCON_PRESETCTRL0_WKT_RST_N_MASK);

	WKT->CTRL 				= WKT_CTRL_CLKSEL_MASK; // (Choose Low Power Clock using Bit 0)
	WKT->COUNT 				= WKT_RELOAD; // Start WKT, counts down from WKT_RELOAD and then interrupt

	NVIC_EnableIRQ(WKT_IRQn); // Enable the WKT interrupt in the NVIC
	__enable_irq();
}
void WKT_IRQHandler(void)
{
	WKT->CTRL |=WKT_CTRL_ALARMFLAG_MASK;
	WKT->COUNT = WKT_RELOAD;
	GPIO->NOT[0] = (1UL<<LED2);
	return;
}
