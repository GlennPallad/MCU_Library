/**
 ******************************************************************************
 * @file 		spi.h
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file is header of spi.c.
 ******************************************************************************
 * @attention
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * <h2><center>COPYRIGHT &copy; 2018 WULFRIC LEE </center></h2>
 ******************************************************************************
 */

#ifndef WULFRIC_SPI_FLASH_H
#define WULFRIC_SPI_FLASH_H

#include "stm32f10x.h"

/* Print dubug info */
#include "wulfric_usart.h"

/* Ports definition */
#define SPI1_NSS_GPIO GPIOC

/* Pins definition */
#define SPI1_NSS_PIN GPIO_Pin_0
#define SPI1_SCK_PIN GPIO_Pin_5
#define SPI1_MISO_PIN GPIO_Pin_6
#define SPI1_MOSI_PIN GPIO_Pin_7

/* Time out */
#define SPI_FLAG_TIMEOUT			((uint32_t)0x1000)

void SPI_Configuration(void);
uint8_t SPI1_SendByte(uint8_t byte);
void select_SPI1_NSS(void);
void deselect_SPI1_NSS(void);

/* Error Soft Handler */
uint8_t SPI1_Timeout_Soft_Handler(void);

#endif
