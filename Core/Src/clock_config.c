#include "clock_config.h"
#include "stm32f4xx_hal_rcc.h"
//static RCC_ClocksTypeDef RCC_Clocks;

/*
 *
 void setup_clock(void)
{

    // Resets the clock configuration to the default reset state
//    RCC_DeInit();
//
//    // Enable external crystal (HSE)
//    RCC_HSEConfig(RCC_HSE_ON);
//    // Wait until HSE ready to use or not
//    ErrorStatus errorStatus = RCC_WaitForHSEStartUp();
//
//    if (errorStatus == SUCCESS)
//    {
//        // Configure the PLL for 168MHz SysClk and 48MHz for USB OTG, SDIO
//        RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
//        // Enable PLL
//        RCC_PLLCmd(ENABLE);
//        // Wait until main PLL clock ready
//        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
//
//        // Set flash latency
//        FLASH_SetLatency(FLASH_Latency_5);
//
//        // AHB 168MHz
//        RCC_HCLKConfig(RCC_SYSCLK_Div1);
//        // APB1 42MHz
//        RCC_PCLK1Config(RCC_HCLK_Div4);
//        // APB2 84 MHz
//        RCC_PCLK2Config(RCC_HCLK_Div2);
//
//        // Set SysClk using PLL
//        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//    }
//    else
//    {
//        // Do something to indicate that error clock configuration
//        while (1);
//    }
//
//    SystemCoreClockUpdate();

	RCC_HSEConfig(RCC_HSE_ON);
	while( RCC_WaitForHSEStartUp() == ERROR ){}
	RCC_PLLConfig(RCC_PLLSource_HSE, M_DIVIDER, N_MULIPLICATOR, P_DIVIDER, Q_DIVIDER);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_PLLCmd(ENABLE);
//	RCC_DeInit();
//	 SystemCoreClockUpdate();
	//SysTick config

//	RCC_ClocksTypeDef RCC_Clocks;
//	RCC_GetClocksFreq(&RCC_Clocks);
//	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	//SysTick config
}

*/



void delay_milis1(uint32_t milisecond)
{
//	RCC_GetClocksFreq(&RCC_Clocks);
//	timing_dalay = RCC_Clocks.HCLK_Frequency/1000 * milisecond;
//	while(timing_dalay--);
}

/*
void delay_milis(uint32_t milisecond)
{
//	RCC_GetClocksFreq(&RCC_Clocks);
//	timing_dalay = RCC_Clocks.HCLK_Frequency/10000000 * microsecond;
//	while(timing_dalay--);
//	timing_dalay = microsecond;
//	while(timing_dalay);

	RCC_GetClocksFreq(&RCC_Clocks);
	timing_dalay = RCC_Clocks.HCLK_Frequency/10000 * milisecond;
	while(timing_dalay--);

//	milisecond*=800000;
//	while(milisecond--);
}
*/


void setup_clock_for_GPIO(GPIO_TypeDef* GPIOx, FunctionalState state)
{
	if (state) {
		switch ((int)GPIOx) {
				case (int)GPIOA:{
					__HAL_RCC_GPIOA_CLK_ENABLE();
					break;}
				case (int)GPIOB:{
					__HAL_RCC_GPIOB_CLK_ENABLE();
					break;}
				case (int)GPIOC:{
					__HAL_RCC_GPIOC_CLK_ENABLE();
					break;}
				case (int)GPIOD:{
					__HAL_RCC_GPIOD_CLK_ENABLE();
					break;}
				case (int)GPIOE: {
					__HAL_RCC_GPIOE_CLK_ENABLE();
					break; }
				default:
					break;
			}
	}
	else {
		switch ((int)GPIOx) {
						case (int)GPIOA:{
							__HAL_RCC_GPIOA_CLK_DISABLE();
							break;}
						case (int)GPIOB:{
							__HAL_RCC_GPIOB_CLK_DISABLE();
							break;}
						case (int)GPIOC:{
							__HAL_RCC_GPIOC_CLK_DISABLE();
							break;}
						case (int)GPIOD:{
							__HAL_RCC_GPIOD_CLK_DISABLE();
							break;}
						case (int)GPIOE:{
							__HAL_RCC_GPIOE_CLK_DISABLE();
							break;}
						default:
							break;
					}
	}


}

