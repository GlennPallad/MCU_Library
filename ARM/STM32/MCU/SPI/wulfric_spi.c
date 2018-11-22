/**
 ******************************************************************************
 * @file 		spi.c
 * @author 		Wulfric Lee
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file is SPI driver.
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

#include "wulfric_spi.h"

void SPI_Configuration(void){
	deselect_SPI1_NSS();
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0x0007; /*!< 0x0007 is reset value */
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_SendByte(uint8_t byte){
	// Check TX FIFO Status
	uint32_t SPI_Timeout = SPI_FLAG_TIMEOUT;
	// Wait until TX FIFO is empty
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
	{
		if((SPI_Timeout--) == 0){
			return SPI1_Timeout_Soft_Handler();
		}
	}
	// Send data to TX FIFO
	SPI_I2S_SendData(SPI1, byte);
	// Wait until RX FIFO not empty
	SPI_Timeout = SPI_FLAG_TIMEOUT;
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
	{
		if((SPI_Timeout--) == 0){
			return SPI1_Timeout_Soft_Handler();
		}
	}
	return SPI_I2S_ReceiveData(SPI1);
}

/**
 * @brief Select SPI1 slave device by setting NSS low.
 */
void select_SPI1_NSS(void){
	GPIO_ResetBits(SPI1_NSS_GPIO, SPI1_NSS_PIN);
}

/**
 * @brief Deselect SPI1 slave device by setting NSS high.
 */
void deselect_SPI1_NSS(void){
	GPIO_SetBits(SPI1_NSS_GPIO, SPI1_NSS_PIN);
}

/**
 * @brief Error Soft Handler.
 */
uint8_t SPI1_Timeout_Soft_Handler(void){
	char str[] = "SPI Time error!\n";
	USART_SendString(USART1, str);
	return 0;
}
