#include "glenn_spi.h"

void SPI_Configuration(void){
	SPI_InitTypeDef SPI_InitStructure;
	deselect_SPI1_NSS();
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0x0007; /*!< 0x0007 is reset value */
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_SendByte(uint8_t byte){
	// Check TX FIFO Status
	uint32_t SPITimeout = SPI_FLAG_TIMEOUT;
	// Wait until TX FIFO is empty
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
	{
		if((SPITimeout--) == 0){
			return SPI1_Timeout_Soft_Handler();
		}
	}
	// Send data to TX FIFO
	SPI_I2S_SendData(SPI1, byte);
	// Wait until RX FIFO not empty
	SPITimeout = SPI_FLAG_TIMEOUT;
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
	{
		if((SPITimeout--) == 0){
			return SPI1_Timeout_Soft_Handler();
		}
	}
	return SPI_I2S_ReceiveData(SPI1);
}

/**
 * @brief Select SPI1 slave device by setting NSS low.
 */
void select_SPI1_NSS(void){
	GPIO_ResetBits(GPIOC, SPI1_NSS_PIN);
}

/**
 * @brief Deselect SPI1 slave device by setting NSS high.
 */
void deselect_SPI1_NSS(void){
	GPIO_SetBits(GPIOC, SPI1_NSS_PIN);
}

/**
 * @brief Error Soft Handler.
 */
uint8_t SPI1_Timeout_Soft_Handler(void){
	char str[] = "SPI Time error!\n";
	USART_SendString(USART1, str);
	return 0;
}
