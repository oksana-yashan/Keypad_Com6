#ifndef DRIVERS_CLOCK_CONFIG_H_
#define DRIVERS_CLOCK_CONFIG_H_

#pragma once
#include <stdint.h>
#include <stm32f4xx.h>
#include <stm32f4xx_hal_rcc.h>

#define M_DIVIDER 		(4) 	// First divider after PLL source MUX
#define N_MULIPLICATOR 	(168) 	// Multiplicator after M_DIVIDER
#define P_DIVIDER 		(2) 	//
#define Q_DIVIDER 		(8) 	//

volatile uint32_t timing_dalay;

//void setup_clock(void);

void delay_milis1(uint32_t milisecond);

//void delay_milis(uint32_t microsecond);

void setup_clock_for_GPIO(GPIO_TypeDef* GPIOx, FunctionalState state);


#endif /* DRIVERS_CLOCK_CONFIG_H_ */
