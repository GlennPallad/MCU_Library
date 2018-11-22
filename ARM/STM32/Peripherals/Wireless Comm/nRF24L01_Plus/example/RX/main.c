/* Standard includes. */
#include "stm32f10x.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Peripherals includes. */
#include "wulfric_rcc.h"
#include "wulfric_gpio.h"
#include "wulfric_nvic.h"
#include "wulfric_usart.h"
#include "wulfric_spi.h"
#include "wulfric_exti.h"
#include "led_pc13.h"
#include "nrf24l01p.h"

/* Algorithms includes */
#include "num_to_ascii.h"

void vTask1( void *pvParameters );

/*
 * Configure the clocks and other peripherals as required by the demo.
 */
static void prvSetupHardware( void );

int main(int argc, char const *argv[])
{
	prvSetupHardware();

	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	EXTI_Configuration();
	USART_Configuration();
	USART_SendString(USART1, "USART1 Normal.\n");
	SPI_Configuration();
	GPIOC_Pin13_Enable();

	NRF_Configuration(nrf_PRX_MODE);

	xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL );

	/* Start the scheduler. */
	vTaskStartScheduler();
	
	return 0;
}

void vTask1( void *pvParameters )
{
	// uint8_t flicker = 1;
	TickType_t xDelay3000ms = pdMS_TO_TICKS(3000);
	/* As per most tasks, this task is implemented in an infinite loop. */
	for ( ; ; )
	{
		vTaskDelay(xDelay3000ms);
	}
}

static void prvSetupHardware( void )
{
	/* Start with the clocks in their expected state. */
	RCC_DeInit();

	/* Enable HSE (high speed external clock). */
	RCC_HSEConfig( RCC_HSE_ON );

	/* Wait till HSE is ready. */
	while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET )
	{
	}

	/* 2 wait states required on the flash. */
	*( ( unsigned long * ) 0x40022000 ) = 0x02;

	/* HCLK = SYSCLK */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2 = HCLK */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* PLLCLK = 8MHz * 9 = 72 MHz. */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

	/* Wait till PLL is used as system clock source. */
	while( RCC_GetSYSCLKSource() != 0x08 )
	{
	}

	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
		| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE );

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );

}
