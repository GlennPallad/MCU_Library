#include "W25Q64.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    

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
