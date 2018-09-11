#ifndef GLENN_SPI_FLASH_H
#define GLENN_SPI_FLASH_H

#include "stm32f10x.h"

/* Print dubug info */
#include "glenn_usart.h"

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
