/**
  ******************************************************************************
  * @file    led_pc13.h
  * @author  Wulfric Lee
  * @version V0.0.0
  * @date    8 August, 2018
  * @brief   This file enables GPIOC_Pin13.
  ******************************************************************************
  * @attention
  *
  * <h2><center> COPYRIGHT &copy; 2018 WULFRIC LEE ALL RIGHTS RESERVED </center></h2>
  ******************************************************************************
  */

#ifndef LED_PC13_H
#define LED_PC13_H 

#include "stm32f10x.h"

void GPIOC_Pin13_Enable(void);
void GPIOC_Pin13_High(void);
void GPIOC_Pin13_Low(void);

#endif
