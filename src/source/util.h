#include "fsl_SSD1306.h"
#include "DAC8568.h"
#include "timers.h"
#include "enums.h"

#define 	ENCODER_A 		(14U)
#define 	ENCODER_B 		(7U)

/*! @brief Initialize the board, customize to initialize relevant peripherals
 *
 * This function turns on clocks, gpios, etc..
 */
void Board_Init(void);

/*! @brief Initialize the master clock and set rate
 *
 * This function turns on clocks
 */
void Board_ClkInit(void);

/*! @brief Initialize the board, customize to initialize relevant peripherals
 *
 * This function turns on clocks, gpios, etc..
 */
void Board_GPIOInit(void);

/*! @brief Initialize level or edge sensitive interrupts
 *
 * This function Set or Clear a group of pixels in the buffer for the Display.
 *
 * @param channel	Interrupt channel to assign.
 * @param pin		Pin to assign to the interrupt
 * @param edge		Set level or edge sensitivity, as well as which edge is sensitive
 */
void Board_IRQInit(int channel, uint32_t pin, uint8_t edge);

uint32_t GPIO_GetVal(uint32_t pin);

void GPIO_PinConfig(uint32_t pin, uint8_t direction);

void GPIO_OutputLow(uint32_t pin);

void GPIO_OutputHigh(uint32_t pin);

void Delay_us(uint32_t microseconds);

void Delay_s(uint32_t seconds);
