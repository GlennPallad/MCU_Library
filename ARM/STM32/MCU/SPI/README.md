# Example
## Read DeviceID from W25Q64(Flash)
*main.c*

```C
#include "wulfric_rcc.h"
#include "wulfric_gpio.h"
#include "wulfric_nvic.h"
#include "wulfric_usart.h"
#include "wulfric_spi.h"
#include "W25Q64.h"

int main(int argc, char const *argv[])
{
	u32 DeviceID;
	char temp[36];
	prvSetupHardware();
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	USART_Configuration();
	SPI_Configuration();
	// Read Device ID
	USART_SendString(USART1, "USART1 Normal.\n");
	DeviceID = SPI_FLASH_ReadDeviceID();
	numberToASCIIBin(DeviceID, temp, sizeof(temp));
	USART_SendString(USART1, temp);

	return 0;
}

```

<br>
*wulfric_rcc.c*

```C
#include "wulfric_rcc.h"

void RCC_Configuration(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}

```

<br>
*wulfric_gpio.c*

```C
#include "wulfric_gpio.h"
#include "wulfric_spi.h"

void GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* USART: Tx Rx */
	// Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* SPI: GPIO A, C and RCC Configurations */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI1_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

```

<br>
*wulfric_nvic.c*

```C
#include "wulfric_nvic.h"

/**
 * @brief This is a setting example, where EXTI0_IRQn, EXTI15_10_IRQn were
 * 		enabled, modify this to meet your needs.
 * 		Find all NVIC_IRQChannel in stm32f10x.h.
 * 		Define your Handlers in stm32f10x_it.c.
 */
void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

```

<br>
*wulfric_spi.c*

```C
#include "wulfric_spi.h"

void SPI_Configuration(void){
	deselect_SPI1_NSS();
	/* SPI Configuration */
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

```

<br>
*W25Q64.c*

```C
#include "W25Q64.h"

static __IO uint32_t  SPI_Timeout = SPIT_LONG_TIMEOUT;

void SPI_FLASH_WriteEnable(void){
	select_SPI1_NSS();
	SPI1_SendByte(W25X_WriteEnable);
	deselect_SPI1_NSS();
}

/**
 * @brief Wait until WIP(busy) is 0, that is, Flash finished writing.
 */
void SPI_FLASH_WaitForWriteEnd(void){
	u8 FLASH_Status = 0;
	select_SPI1_NSS();
	SPI1_SendByte(W25X_ReadStatusReg);
	FLASH_Status = SPI1_SendByte(Dummy_Byte);
	while(FLASH_Status & WIP_Flag);
	deselect_SPI1_NSS();
}

/**
 * @brief Erase one sector.
 * @param SectorAddr: the sector you want to erase.
 */
void SPI_FLASH_SectorErase(u32 SectorAddr){
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	// Erase sector
	select_SPI1_NSS();
	SPI1_SendByte(W25X_SectorErase);
	// A23 ~ A16
	SPI1_SendByte((SectorAddr & 0xFF0000) >> 16);
	// A15 ~ A8
	SPI1_SendByte((SectorAddr & 0xFF00) >> 8);
	// A7 ~ A0
	SPI1_SendByte((SectorAddr & 0xFF) >> 8);
	deselect_SPI1_NSS();
	SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief Erase whole chip.
 */
void SPI_FLASH_BulkErase(void){
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	// Erase chip
	select_SPI1_NSS();
	SPI1_SendByte(W25X_ChipErase);
	deselect_SPI1_NSS();
	SPI_FLASH_WaitForWriteEnd();
}

void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite){

}

void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite){

}

void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead){

}

u32 SPI_FLASH_ReadID(void){
	u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	select_SPI1_NSS();
	SPI1_SendByte(W25X_JedecDeviceID);
	Temp0 = SPI1_SendByte(Dummy_Byte);
	Temp1 = SPI1_SendByte(Dummy_Byte);
	Temp2 = SPI1_SendByte(Dummy_Byte);
	deselect_SPI1_NSS();
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}

u32 SPI_FLASH_ReadDeviceID(void){
	u32 Temp = 0;
	select_SPI1_NSS();
	SPI1_SendByte(W25X_DeviceID);
	SPI1_SendByte(Dummy_Byte);
	SPI1_SendByte(Dummy_Byte);
	SPI1_SendByte(Dummy_Byte);
	Temp = SPI1_SendByte(Dummy_Byte);
	deselect_SPI1_NSS();
	return Temp;
}

void SPI_FLASH_StartReadSequence(u32 ReadAddr){

}

void SPI_Flash_PowerDown(void){
	select_SPI1_NSS();
	SPI1_SendByte(W25X_PowerDown);
	deselect_SPI1_NSS();
}

void SPI_Flash_WAKEUP(void){
	select_SPI1_NSS();
	SPI1_SendByte(W25X_ReleasePowerDown);
	deselect_SPI1_NSS();
}

```
