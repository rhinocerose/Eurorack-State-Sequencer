/*
 * enums.h
 *
 *  Created on: Mar. 3, 2020
 *      Author: ashar
 */

#ifndef ENUMS_H_
#define ENUMS_H_

/*! @brief Direction options for GPIO. */
enum _GPIO_Direction
{
	gpio_Output_ON,
	gpio_Output_OFF,
	gpio_Input
};

/*! @brief Level or edge sensitivity for interrupts. */
enum _IRQ_Edge_Mode
{
	irq_edge_Rising,
	irq_edge_Falling,
	irq_edge_Both,
	irq_Level
};


#endif /* ENUMS_H_ */
