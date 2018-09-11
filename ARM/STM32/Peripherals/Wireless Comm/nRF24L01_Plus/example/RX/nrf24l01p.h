/**
 ******************************************************************************
 * @file 		nrf24l01p.h
 * @author 		Glenn Pallad
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file is header of nrf24l01p.c.
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
 * <h2><center>COPYRIGHT &copy; 2018 GLENN PALLAD </center></h2>
 ******************************************************************************
 */

#ifndef NRF24L01P_H
#define NRF24L01P_H

#include "stm32f10x.h"

enum nrf_Mode
{
	nrf_PTX_MODE,
	nrf_PRX_MODE
};

typedef enum nrf_RX_Event
{
	nrf_IDLE,
	nrf_RX_DR,
	nrf_TX_DS,
	nrf_MAX_RT
}nrf_RX_Event;

/** @defgroup User_Configurations 
 *  @{
 */
#define BIT_RF_CH 						0b00000010 				/*!< Reset value */
#define BIT_TX_ADDR 					0xE7E7E7E7E7 			/*!< Reset value 0xE7E7E7E7E7 */
#define BIT_RX_ADDR_P0 					0xE7E7E7E7E7 			/*!< Reset value 0xE7E7E7E7E7 */
#define BIT_RX_ADDR_P1 					0xC2C2C2C2C2 			/*!< Reset value 0xC2C2C2C2C2 */
#define BIT_RX_ADDR_P2 					0xC3 					/*!< Reset value 0xC3 */
#define BIT_RX_ADDR_P3 					0xC4					/*!< Reset value 0xC4 */
#define BIT_RX_ADDR_P4 					0xC5					/*!< Reset value 0xC5 */
#define BIT_RX_ADDR_P5 					0xC6					/*!< Reset value 0xC6 */
#define BIT_RX_PW_P0 					0b00111111 				/*!< Reset value 0b00000000 */
#define BIT_RX_PW_P1 					0b00111111 				/*!< Reset value 0b00000000 */
#define BIT_RX_PW_P2 					0b00111111 				/*!< Reset value 0b00000000 */
#define BIT_RX_PW_P3 					0b00111111 				/*!< Reset value 0b00000000 */
#define BIT_RX_PW_P4 					0b00111111 				/*!< Reset value 0b00000000 */
#define BIT_RX_PW_P5 					0b00111111 				/*!< Reset value 0b00000000 */
#define BIT_RF_SETUP_RF_DR 				RF_DR_250KBPS 			
#define BIT_RF_SETUP_RF_PWR 			RF_PWR_0DBM 			/*!< Reset value */
#define BIT_SETUP_RETR_ARC 				BIT_SETUP_RETR_ARC_3 	/*!< Reset value */

#define NRF_CE_PORT 					GPIOA
#define NRF_CE_Pin 						GPIO_Pin_2
#define NRF_IRQ_Pin 					GPIO_Pin_1
/** @}*/

/* --- Register Map --- */
#define REG_CONFIG 						0x00
#define REG_EN_AA 						0x01 	/*!< Set this in PRX side only */
#define REG_EN_RXADDR 					0x02
#define REG_SETUP_AW 					0x03
#define REG_SETUP_RETR 					0x04
#define REG_RF_CH 						0x05
#define REG_RF_SETUP 					0x06
#define REG_STATUS 						0x07
#define REG_OBSERVE_TX 					0x08
#define REG_RPD 						0x09
#define REG_RX_ADDR_P0 					0x0A
#define REG_RX_ADDR_P1 					0x0B
#define REG_RX_ADDR_P2 					0x0C
#define REG_RX_ADDR_P3 					0x0D
#define REG_RX_ADDR_P4 					0x0E
#define REG_RX_ADDR_P5 					0x0F
#define REG_TX_ADDR 					0x10
#define REG_RX_PW_P0 					0x11
#define REG_RX_PW_P1 					0x12
#define REG_RX_PW_P2 					0x13
#define REG_RX_PW_P3 					0x14
#define REG_RX_PW_P4 					0x15
#define REG_RX_PW_P5 					0x16
#define REG_FIFO_STATUS 				0x17
#define REG_DYNPD 						0x1C
#define REG_FEATURE 					0x1D

/* --- Bits Mnemonics --- */
/* REG_CONFIG */
#define BIT_CONFIG_MASK_RX_DR 			0b01000000
#define BIT_CONFIG_MASK_TX_DS 			0b00100000
#define BIT_CONFIG_MASK_MAX_RT 			0b00010000
#define BIT_CONFIG_EN_CRC 				0b00001000 	/*!< Reset value */
#define BIT_CONFIG_CRCO 				0b00000100
#define BIT_CONFIG_PWR_UP 				0b00000010
#define BIT_CONFIG_PRIM_RX 				0b00000001
/* REG_EN_AA */
#define BIT_EN_AA_ENAA_P5 				0b00100000 	/*!< Reset value */
#define BIT_EN_AA_ENAA_P4 				0b00010000 	/*!< Reset value */
#define BIT_EN_AA_ENAA_P3 				0b00001000 	/*!< Reset value */
#define BIT_EN_AA_ENAA_P2 				0b00000100 	/*!< Reset value */
#define BIT_EN_AA_ENAA_P1 				0b00000010 	/*!< Reset value */
#define BIT_EN_AA_ENAA_P0 				0b00000001 	/*!< Reset value */
/* REG_EN_RXADDR */
#define BIT_EN_RXADDR_ERX_P5 			0b00100000
#define BIT_EN_RXADDR_ERX_P4 			0b00010000
#define BIT_EN_RXADDR_ERX_P3 			0b00001000
#define BIT_EN_RXADDR_ERX_P2 			0b00000100
#define BIT_EN_RXADDR_ERX_P1 			0b00000010 	/*!< Reset value */
#define BIT_EN_RXADDR_ERX_P0 			0b00000001 	/*!< Reset value */
/* REG_SETUP_AW */
#define BIT_SETUP_AW_3_BYTES 			0b00000001
#define BIT_SETUP_AW_4_BYTES 			0b00000010
#define BIT_SETUP_AW_5_BYTES 			0b00000011 	/*!< Reset value */
/* REG_SETUP_RETR */
// This was set according to different bandwidths, in setARD(void).
#define BIT_SETUP_RETR_ARD_250_UM 		0x00 	/*!< Reset value */
#define BIT_SETUP_RETR_ARD_500_UM 		0x10 	/*!< Mostly used value */
#define BIT_SETUP_RETR_ARD_750_UM 		0x20
#define BIT_SETUP_RETR_ARD_1000_UM 		0x30
#define BIT_SETUP_RETR_ARD_1250_UM 		0x40
#define BIT_SETUP_RETR_ARD_1500_UM 		0x50 	/*!< Mostly used value */
#define BIT_SETUP_RETR_ARD_1750_UM 		0x60
#define BIT_SETUP_RETR_ARD_2000_UM 		0x70
#define BIT_SETUP_RETR_ARD_2250_UM 		0x80
#define BIT_SETUP_RETR_ARD_2500_UM 		0x90
#define BIT_SETUP_RETR_ARD_2750_UM 		0xA0
#define BIT_SETUP_RETR_ARD_3000_UM 		0xB0
#define BIT_SETUP_RETR_ARD_3250_UM 		0xC0
#define BIT_SETUP_RETR_ARD_3500_UM 		0xD0
#define BIT_SETUP_RETR_ARD_3750_UM 		0xE0
#define BIT_SETUP_RETR_ARD_4000_UM 		0xF0
// Set BIT_SETUP_RETR_ARC in @ref User_Configurations
#define BIT_SETUP_RETR_ARC_DISABLE 		0x00
#define BIT_SETUP_RETR_ARC_1 			0x01
#define BIT_SETUP_RETR_ARC_2 			0x02
#define BIT_SETUP_RETR_ARC_3 			0x03 	/*!< Reset value */
#define BIT_SETUP_RETR_ARC_4 			0x04
#define BIT_SETUP_RETR_ARC_5 			0x05
#define BIT_SETUP_RETR_ARC_6 			0x06
#define BIT_SETUP_RETR_ARC_7 			0x07
#define BIT_SETUP_RETR_ARC_8 			0x08
#define BIT_SETUP_RETR_ARC_9 			0x09
#define BIT_SETUP_RETR_ARC_10 			0x0A
#define BIT_SETUP_RETR_ARC_11 			0x0B
#define BIT_SETUP_RETR_ARC_12 			0x0C
#define BIT_SETUP_RETR_ARC_13 			0x0D
#define BIT_SETUP_RETR_ARC_14 			0x0E
#define BIT_SETUP_RETR_ARC_15 			0x0F
/* REG_RF_CH */
// Set this up in @ref User_Configurations
/* REG_RF_SETUP */
#define BIT_RF_SETUP_CONT_WAVE 			0b10000000
#define BIT_RF_SETUP_RF_DR_LOW 			0b00100000
#define BIT_RF_SETUP_PLL_LOCK 			0b00010000
#define BIT_RF_SETUP_RF_DR_HIGH 		0b00001000 	/*!< Reset value
														Encoding: [RF_DR_LOW, RF_DR_HIGH]
														‘00’ – 1Mbps
														‘01’ – 2Mbps
														‘10’ – 250kbps
														‘11’ – Reserved */
#define RF_DR_250KBPS 					BIT_RF_SETUP_RF_DR_LOW
#define RF_DR_1MBPS 					BIT_RF_SETUP_RF_DR_LOW & 0b00000000
#define RF_DR_2MBPS 					BIT_RF_SETUP_RF_DR_HIGH 	/*!< Reset value */
// Set BIT_RF_SETUP_RF_PWR in @ref User_Configurations
#define RF_PWR_MINUS_18DBM 				0b00000000
#define RF_PWR_MINUS_12DBM 				0b00000010
#define RF_PWR_MINUS_6DBM 				0b00000100
#define RF_PWR_0DBM 					0b00000110 	/*!< Reset value
														Set RF output power in TX mode
														'00' – -18dBm
														'01' – -12dBm
														'10' – -6dBm
														'11' – 0dBm */
/* REG_STATUS */
#define BIT_STATUS_RX_DR 				0b01000000
#define BIT_STATUS_TX_DS 				0b00100000
#define BIT_STATUS_MAX_RT 				0b00010000
#define BIT_STATUS_RX_P_NO 				0b00001110 	/*!< Reset value */
#define BIT_STATUS_TX_FULL 				0b00000001
/* REG_OBSERVE_TX */
#define BIT_OBSERVE_TX_PLOS_CNT 		0b11110000
#define BIT_OBSERVE_TX_ARC_CNT 			0b00001111
/* REG_RPD */
#define BIT_RPD 						0b00000001
/* REG_RX_ADDR_P0 */
// Set this up in @ref User_Configurations
/* REG_RX_ADDR_P1 */
// Set this up in @ref User_Configurations
/* REG_RX_ADDR_P2 */
// Set this up in @ref User_Configurations
/* REG_RX_ADDR_P3 */
// Set this up in @ref User_Configurations
/* REG_RX_ADDR_P4 */
// Set this up in @ref User_Configurations
/* REG_RX_ADDR_P5 */
// Set this up in @ref User_Configurations
/* REG_TX_ADDR */
// Set this up in @ref User_Configurations
/* REG_RX_PW_P0 */
// Set this up in @ref User_Configurations 			
/* REG_RX_PW_P1 */
// Set this up in @ref User_Configurations
/* REG_RX_PW_P2 */
// Set this up in @ref User_Configurations
/* REG_RX_PW_P3 */
// Set this up in @ref User_Configurations
/* REG_RX_PW_P4 */
// Set this up in @ref User_Configurations
/* REG_RX_PW_P5 */
// Set this up in @ref User_Configurations
/* REG_FIFO_STATUS */
#define BIT_FIFO_STATUS_TX_REUSE 		0b01000000
#define BIT_FIFO_STATUS_TX_FULL 		0b00100000
#define BIT_FIFO_STATUS_TX_EMPTY 		0b00010000 	/*!< Reset value */
#define BIT_FIFO_STATUS_RX_FULL 		0b00000010
#define BIT_FIFO_STATUS_RX_EMPTY 		0b00000001 	/*!< Reset value */
/* REG_DYNPD */
#define BIT_DYNPD_DPL_P5 				0b00100000
#define BIT_DYNPD_DPL_P4 				0b00010000
#define BIT_DYNPD_DPL_P3 				0b00001000
#define BIT_DYNPD_DPL_P2 				0b00000100
#define BIT_DYNPD_DPL_P1 				0b00000010
#define BIT_DYNPD_DPL_P0 				0b00000001
/* REG_FEATURE */
#define BIT_FEATURE_EN_DPL 				0b00000100
#define BIT_FEATURE_EN_ACK_PAY 			0b00000010
#define BIT_FEATURE_EN_DYN_ACK 			0b00000001

/* --- SPI commands --- */
#define CMD_R_REGISTER 					0b00000000	/*!< OR this with 5-bits Register address. */
#define CMD_W_REGISTER 					0b00100000	/*!< OR this with 5-bits Register address. */
#define CMD_R_RX_PAYLOAD 				0b01100001
#define CMD_W_TX_PAYLOAD 				0b10100000 
#define CMD_FLUSH_TX 					0b11100001
#define CMD_FLUSH_RX 					0b11100010
#define CMD_REUSE_TX_PL 				0b11100011
#define CMD_R_RX_PL_WID 				0b01100000
#define CMD_W_ACK_PAYLOAD 				0b10101000	/*!< OR this with NRF_PIPEx 
														where x can be (0..5). */
#define NRF_PIPE0 						0b00000000
#define NRF_PIPE1 						0b00000001
#define NRF_PIPE2 						0b00000010
#define NRF_PIPE3 						0b00000011
#define NRF_PIPE4 						0b00000100
#define NRF_PIPE5 						0b00000101
#define CMD_W_TX_PAYLOAD_NOACK 			0b10110000
#define CMD_NOP 						0b11111111

/* --- Functions --- */
#define SELECT_NRF_NSS() 				select_SPI1_NSS()
#define DESELECT_NRF_NSS() 				deselect_SPI1_NSS()
#define SENDBYTE_NRF(byte) 				SPI1_SendByte(byte)

void NRF_Configuration(enum nrf_Mode mode);

uint8_t nrf_writeReg(uint8_t regAddr, uint8_t value);
uint8_t nrf_readReg(uint8_t regAddr);
void nrf_txPayload(uint8_t* pPayload, uint8_t len);
uint8_t nrf_rxPayload(uint8_t* pPayload);
void nrf_txNoAckPayload(uint8_t* pPayload, uint8_t len);
void nrf_txAck_with_Payload(uint8_t* pPayload, uint8_t len);
nrf_RX_Event nrf_checkEvent_and_ReceivePayload(uint8_t* pPayload, uint8_t* pLength);
uint8_t nrf_sendPayload_and_WaitAck(uint8_t* pPayload, uint8_t payloadLength, uint8_t* pAckPayload, uint8_t* pAckPayloadLength);

void nrf_CE_Low(void);
void nrf_CE_High(void);

#endif
