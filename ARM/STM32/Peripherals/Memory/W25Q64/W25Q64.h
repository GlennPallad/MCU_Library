#ifndef W25Q64
#define W25Q64

#include "stm32f10x.h"

#include "wulfric_spi.h"

// #define sFLASH_ID 0xEF3015 /*!< W25X16 */
// #define sFLASH_ID 0xEF4015 /*!< W25Q16 */
#define sFLASH_ID 0xEF4017 /*!< W25Q64 */
// #define sFLASH_ID 0xEF4018 */* W 25Q128 */

#define SPI_FLASH_PageSize			256
#define SPI_FLASH_PerWritePageSize	256

/* Commands */
#define W25X_WriteEnable			0x06
#define W25X_WriteDisable			0x04
#define W25X_ReadStatusReg			0x05
#define W25X_WriteStatusReg			0x01
#define W25X_ReadData				0x03
#define W25X_FastReadData			0x0B
#define W25X_FastReadDual			0x3B
#define W25X_PageProgram			0x02
#define W25X_BlockErase				0xD8
#define W25X_SectorErase			0x20
#define W25X_ChipErase				0xC7
#define W25X_PowerDown				0xB9
#define W25X_ReleasePowerDown		0xAB
#define W25X_DeviceID				0xAB
#define W25X_ManufactDeviceID		0x90
#define W25X_JedecDeviceID			0x9F

/* WIP(busy) */
#define WIP_Flag					0x01
#define Dummy_Byte					0xFF

/* Time out */
#define SPIT_FLAG_TIMEOUT			((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT			((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/* Info Output */
// #define FLASH_DEBUG_ON				1

// #define FLASH_INFO(fmt,arg...)		printf("<<-FLASH-INFO->> "fmt"\n",##arg)
// #define FLASH_ERROR(fmt,arg...)		printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
// #define FLASH_DEBUG(fmt,arg...)		if(FLASH_DEBUG_ON)\
// 										printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);

void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);

#endif
