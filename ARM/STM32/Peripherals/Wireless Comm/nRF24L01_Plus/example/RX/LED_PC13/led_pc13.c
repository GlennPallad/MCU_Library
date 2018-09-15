/**
 ******************************************************************************
 * @file    led_pc13.c
 * @author  Glenn Pallad
 * @version V0.0.0
 * @date    8 August, 2018
 * @brief   This file provides functions manipulate GPIOC_Pin13.
 ******************************************************************************
 * @attention
 *
 * <h2><center> COPYRIGHT &copy; 2018 GLENN PALLAD ALL RIGHTS RESERVED </center></h2>
 ******************************************************************************
 */

#include "led_pc13.h"

/**
 * @brief	Enables GPIOC Pin13.
 * @param	None
 * @retval	None
 */
void GPIOC_Pin13_Enable(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIOC_Pin13_High();   /*!< Init to turn off the light */
}

/**
 * @brief	Set GPIOC Pin13.
 * @note	This will turn off the on-board LED.
 * @param	None
 * @retval	None
 */
void GPIOC_Pin13_High(void){
	GPIOC->BSRR = (uint32_t)GPIO_Pin_13;
}

/**
 * @brief	Set GPIOC Pin13.
 * @note	This will turn on the on-board LED.
 * @param	None
 * @retval	None
 */
void GPIOC_Pin13_Low(void){
	GPIOC->BRR = (uint32_t)GPIO_Pin_13;
}



