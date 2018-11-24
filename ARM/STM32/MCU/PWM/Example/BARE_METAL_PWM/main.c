/* Standard includes. */
#include <stdio.h>

#include "stm32f10x.h"

#include "wulfric_rcc.h"
#include "wulfric_nvic.h"
#include "wulfric_gpio.h"
#include "wulfric_tim.h"
#include "wulfric_usart.h"

char welcome[] = "Welcome to USART1\n";
int main(int argc, char const *argv[])
{
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	TIM_Configuration();
	USART_Configuration();
	USART_SendString(USART1, welcome);

	while(1){
	}
	
	return 0;
}