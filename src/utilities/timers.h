/*
 * timers.h
 *
 *  Created on: Mar. 3, 2020
 *      Author: ashar
 */

#ifndef 	TIMERS_H_
#define 	TIMERS_H_

#include 	"LPC804.h"
#include 	"clock_config.h"

#define 	LED1 		 		(20)  	// One of the LED (PIO0_20)
#define 	LED2 		 		(18)  	// One of the LED (PIO0_18)

#define 	MRT_REPEAT 			(0) 	// Repeat mode for MRT
#define 	MRT_ONESHOT 		(1) 	// One shot mode for MRT
#define 	MRT_ONESHOT_Stall 	(2)		// One shot + bus stall mode for MRT
#define 	MRT_GFLAG0 			(0)		// IRQ Flag 0 (channel 0)
#define 	MRT_GFLAG1 			(1)		// IRQ Flag 1 (channel 1)
#define 	MRT_CHAN0 			(0)		// channel 0 on MRT
#define 	MRT_CHAN1 			(1)		// channel 1 on MRT
#define 	MRT_INIT_CH0 		(300000)//
#define 	MRT_INIT_CH1 		(300000)

#define		SYSTICK_COUNTER 	(12000000)

#define 	WKT_RELOAD 			(100000)

void Timers_Init(void);

void MRT_Enable(void);

//void SysTick_Handler(void);
void Systick_Enable(void);

void WKT_Enable(void);
void WKT_IRQHandler(void);


#endif /* TIMERS_H_ */
