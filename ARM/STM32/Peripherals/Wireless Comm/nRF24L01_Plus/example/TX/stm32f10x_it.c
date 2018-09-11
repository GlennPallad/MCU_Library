/**
	******************************************************************************
	* @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
	* @author  MCD Application Team
	* @version V3.5.0
	* @date    08-April-2011
	* @brief   Main Interrupt Service Routines.
	*          This file provides template for all exceptions handler and 
	*          peripherals interrupt service routine.
	******************************************************************************
	* @attention
	*
	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
	* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
	* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
	* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
	*
	* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
	******************************************************************************
	*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "glenn_usart.h"
#include "bsp_led.h"
#include "nrf24l01p.h"
#include "num_to_ascii.h"

/** @addtogroup STM32F10x_StdPeriph_Template
	* @{
	*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
	* @brief  This function handles NMI exception.
	* @param  None
	* @retval None
	*/
void NMI_Handler(void)
{
}

/**
	* @brief  This function handles Hard Fault exception.
	* @param  None
	* @retval None
	*/
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

/**
	* @brief  This function handles Memory Manage exception.
	* @param  None
	* @retval None
	*/
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

/**
	* @brief  This function handles Bus Fault exception.
	* @param  None
	* @retval None
	*/
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

/**
	* @brief  This function handles Usage Fault exception.
	* @param  None
	* @retval None
	*/
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

/**
	* @brief  This function handles SVCall exception.
	* @param  None
	* @retval None
	*/
void SVC_Handler(void)
{
}

/**
	* @brief  This function handles Debug Monitor exception.
	* @param  None
	* @retval None
	*/
void DebugMon_Handler(void)
{
}

/**
	* @brief  This function handles PendSVC exception.
	* @param  None
	* @retval None
	*/
void PendSV_Handler(void)
{
}

/**
	* @brief  This function handles SysTick Handler.
	* @param  None
	* @retval None
	*/
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
	* @brief  This function handles PPP interrupt request.
	* @param  None
	* @retval None
	*/
/*void PPP_IRQHandler(void)
{
}*/

/**
	* @}
	*/ 

uint8_t payload[15] = {0x48, 0x69, 0x2C, 0x20, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x47, 0x50, 0x21};
							/*!< ASCII of "Hi, This is GP!"
								NOTE: This shouldn't be over 32 bytes. */
uint8_t ackPayload[32];
uint8_t ackPayloadLength = 32;
/**
 * @brief Handler for KEY1. 
 */
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		uint8_t result;
		LED2_ON;
		result = nrf_sendPayload_and_WaitAck(payload, sizeof(payload), ackPayload, &ackPayloadLength);
		if (result == 0)
		{
			LED1_ON;
			LED2_OFF;
		} else {
			LED1_OFF;
			LED2_ON;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**
 * @brief Handler for nRF IRQ.
 */
void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		GPIOA->BSRR = (uint32_t)GPIO_Pin_8;
		TIM_Cmd(TIM6, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

/**
 * @brief Handler for TIM6 IRQ.
 */
void TIM6_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		TIM_Cmd(TIM6, DISABLE);
		/*!< Shut down beep */
		GPIOA->BRR = (uint32_t)GPIO_Pin_8;
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
	}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/