/**
 ******************************************************************************
 * @file 		nrf24l01p.c
 * @author 		Glenn Pallad
 * @version 	V0.8.0_Alpha
 * @date 		August 28, 2018
 * @brief 		This file is nrf24l01p driver.
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
	
#include "nrf24l01p.h"

#include "glenn_spi.h"
#include "glenn_usart.h"

static uint64_t tx_addr = BIT_TX_ADDR;
static uint64_t rx_addr_p0 = BIT_RX_ADDR_P0;

static void writeCmd(uint8_t command);
static uint8_t writeBuff(uint8_t command, uint8_t* pBuff, uint8_t len);
static uint8_t readBuff(uint8_t command, uint8_t* pBuff, uint8_t len);

static void setAddr(enum nrf_Mode mode);
static void setChann(void);
static void setDataRate(void);
static void setPA(void);
static void setARD(void);
static void setARC(void);
static void setMode(enum nrf_Mode mode);
static void check_SPI(void);

/**
 * @brief NRF Configuration.
 * @param mode: this can be one of following values:
 * 		@arg nrf_PTX_MODE
 * 		@arg nrf_PRX_MODE
 * @retval None
 */
void NRF_Configuration(enum nrf_Mode mode)
{
	nrf_CE_Low();
	setAddr(mode);
	setChann();
	setDataRate();
	setPA();
	setARD();
	setARC();
	check_SPI();
	setMode(mode);
}

/**
 * @brief Write command.
 * @param command: SPI command, but can NOT be one of following values:
 * 			CMD_R_REGISTER
 * 			CMD_W_REGISTER
 * 			CMD_W_ACK_PAYLOAD
 * 		which are related to Write or Read.
 * @retval None
 */
void writeCmd(uint8_t command)
{
	SELECT_NRF_NSS();
	SENDBYTE_NRF(command);
	DESELECT_NRF_NSS();
}

/**
 * @brief Write register.
 * @param regAddr: register address, the register can not have
 * 		space more than 1 Byte.
 * @param value: the value you want to write.
 * @retval The value send back to MISO simultaneously.
 */
uint8_t nrf_writeReg(uint8_t regAddr, uint8_t value)
{
	uint8_t status;
	SELECT_NRF_NSS();
	status = SENDBYTE_NRF(CMD_W_REGISTER | regAddr);
	SENDBYTE_NRF(value);
	DESELECT_NRF_NSS();
	return status;
} 

/**
 * @brief Read register.
 * @param regAddr: register address, the register can not have
 * 		space more than 1 Byte.
 * @retval The value send back to MISO.
 */
uint8_t nrf_readReg(uint8_t regAddr)
{
	uint8_t regVal;
	SELECT_NRF_NSS();
	SENDBYTE_NRF(CMD_R_REGISTER | regAddr);
	regVal = SENDBYTE_NRF(CMD_NOP);
	DESELECT_NRF_NSS();
	return regVal;
}

/**
 * @brief Write buffer.
 * @param command: SPI commands.
 * 		Where commands should be related to Write or Read, which can be one of the following values:
 * 			@arg CMD_R_RX_PAYLOAD
 *			@arg CMD_W_TX_PAYLOAD
 * 			@arg CMD_R_RX_PL_WID
 * 			@arg (CMD_W_ACK_PAYLOAD | NRF_PIPEx)
 * 			@arg CMD_W_TX_PAYLOAD_NOACK
 * 		Or can be one of the following values:
 * 			@arg (CMD_R_REGISTER | REG_XXX )
 * 			@arg (CMD_W_REGISTER | REG_XXX )
 * 		Where 
 *			NRF_PIPEx: x can be (0..5).
 * 			REG_XXX are registers that have more than 1 Byte space.
 * @param pBuff: address of the buff you want to write.
 * @param len: length of the buff you want to write.
 * @retval The value send back to MISO simultaneously.
 */
uint8_t writeBuff(uint8_t command, uint8_t* pBuff, uint8_t len)
{
	uint8_t status;
	SELECT_NRF_NSS();
	status = SENDBYTE_NRF(command);
	for (int i = 0; i < len; ++i)
	{
		SENDBYTE_NRF(*(pBuff + i));
	}
	DESELECT_NRF_NSS();
	return status;
}

/**
 * @brief Read buffer.
 * @param command: SPI commands.
 * 		Where commands should be related to Write or Read, which can be one of the following values:
 * 			@arg CMD_R_RX_PAYLOAD
 *			@arg CMD_W_TX_PAYLOAD
 * 			@arg CMD_R_RX_PL_WID
 * 			@arg (CMD_W_ACK_PAYLOAD | NRF_PIPEx)
 * 			@arg CMD_W_TX_PAYLOAD_NOACK
 * 		Or can be one of the following values:
 * 			@arg (CMD_R_REGISTER | REG_XXX )
 * 			@arg (CMD_W_REGISTER | REG_XXX )
 * 		Where 
 *			NRF_PIPEx: x can be (0..5).
 * 			REG_XXX are registers that have more than 1 Byte space.
 * @param pBuff: address of the buff you want to write.
 * @param len: length of the buff you want to write.
 * @retval The value send back to MISO simultaneously.
 */
uint8_t readBuff(uint8_t command, uint8_t* pBuff, uint8_t len)
{
	uint8_t status;
	SELECT_NRF_NSS();
	status = SENDBYTE_NRF(command);
	for (int i = 0; i < len; ++i)
	{
		*(pBuff + i) = SENDBYTE_NRF(CMD_NOP);
	}
	DESELECT_NRF_NSS();
	return status;
}

/**
 * @brief Set Tx and Rx Addresses.
 * @param mode: this can be one of following values:
 * 		@arg nrf_PTX_MODE
 * 		@arg nrf_PRX_MODE
 * @retval None
 */
void setAddr(enum nrf_Mode mode)
{
	if (mode == nrf_PTX_MODE)
	{
		writeBuff(CMD_W_REGISTER | REG_TX_ADDR, (uint8_t* )&tx_addr, 5);
		writeBuff(CMD_W_REGISTER | REG_RX_ADDR_P0, (uint8_t* )&rx_addr_p0, 5);
	} else {	
		writeBuff(CMD_W_REGISTER | REG_RX_ADDR_P0, (uint8_t* )&rx_addr_p0, 5);
	}
}

/**
 * @brief Set RF Channel.
 * @param None
 * @retval None
 */
void setChann(void)
{
	if (BIT_RF_CH <= 123)
	{
		nrf_writeReg(REG_RF_CH, BIT_RF_CH);
	} else {
		USART_SendString(USART1, "Improper channel setting, shouldn't be larger than 123MHz\n\
										(2.525GHz - 2.4GHz - 2MHz(bandwidth protection)).\n");
	}
}

/**
 * @brief Set air data rate.
 * @param None
 * @retval None
 */
void setDataRate(void)
{
	uint8_t reg_rf_setup = nrf_readReg(REG_RF_SETUP);
	reg_rf_setup = reg_rf_setup & 0b11010111; 	/*!< Clear old BIT_RF_SETUP_RF_DR_LOW and BIT_RF_SETUP_RF_DR_HIGH */
	reg_rf_setup |= BIT_RF_SETUP_RF_DR;
	nrf_writeReg(REG_RF_SETUP, reg_rf_setup);
}

/**
 * @brief Set RF power.
 * @param None
 * @retval None
 */
void setPA(void)
{
	uint8_t reg_rf_setup = nrf_readReg(REG_RF_SETUP);
	reg_rf_setup = reg_rf_setup & 0b11111001; 	/*!< Clear old BIT_RF_SETUP_RF_PWR. */
	reg_rf_setup |= BIT_RF_SETUP_RF_PWR;
	nrf_writeReg(REG_RF_SETUP, reg_rf_setup);
}

/**
 * @brief Set ARD(Auto Retransmit Delay).
 * @param None
 * @retval None
 */
void setARD(void)
{
	uint8_t reg_rf_setup, reg_setup_retr;
	reg_rf_setup = nrf_readReg(REG_RF_SETUP);
	reg_setup_retr = nrf_readReg(REG_SETUP_RETR);
	reg_setup_retr = reg_setup_retr & 0x0F; 	/*!< Clear old BIT_SETUP_RETR_ARD. */
	if ((reg_rf_setup & 0b00101000) != RF_DR_250KBPS)
	{
		reg_setup_retr = reg_setup_retr | BIT_SETUP_RETR_ARD_500_UM;
	} else {
		reg_setup_retr = reg_setup_retr | BIT_SETUP_RETR_ARD_1500_UM;
	}
	nrf_writeReg(REG_SETUP_RETR, reg_setup_retr);
}

/**
 * @brief Set ARC(Auto Retransmit Count).
 * @param None
 * @retval None
 */
void setARC(void)
{
	uint8_t reg_setup_retr;
	reg_setup_retr = nrf_readReg(REG_SETUP_RETR);
	reg_setup_retr = reg_setup_retr & 0xF0; 	/*!< Clear old BIT_SETUP_RETR_ARD. */
	reg_setup_retr = reg_setup_retr | BIT_SETUP_RETR_ARC;
	nrf_writeReg(REG_SETUP_RETR, reg_setup_retr);
}

/**
 * @brief Get RPD(Received Power Detector).
 * @param None
 * @retval None
 */
uint8_t getRPD(void)
{
	return nrf_readReg(REG_RPD);
}

/**
 * @brief Get ARC_CNT, count retransmitted packets. The counter is reset
 * 		when transmission of a new packet starts.
 * @param None
 * @retval None
 */
uint8_t getARC_CNT(void)
{
	return nrf_readReg(REG_OBSERVE_TX) & 0x0F;
}

/**
 * @brief Set nrf mode.
 * @param mode: this can be one of following values:
 * 		@arg nrf_PTX_MODE
 * 		@arg nrf_PRX_MODE
 * @retval None
 */
void setMode(enum nrf_Mode mode)
{
	nrf_writeReg(REG_STATUS, BIT_STATUS_RX_DR | BIT_STATUS_TX_DS | BIT_STATUS_MAX_RT); 	/*!< Recover to reset status. */
	if (mode == nrf_PTX_MODE)
	{
		nrf_writeReg(REG_CONFIG, BIT_CONFIG_EN_CRC | BIT_CONFIG_CRCO | BIT_CONFIG_PWR_UP);
		nrf_writeReg(REG_FEATURE, BIT_FEATURE_EN_DPL);
		nrf_writeReg(REG_DYNPD, BIT_DYNPD_DPL_P0);
		writeCmd(CMD_FLUSH_TX);
		writeCmd(CMD_FLUSH_RX);
	} else {
		nrf_writeReg(REG_CONFIG, BIT_CONFIG_EN_CRC | BIT_CONFIG_CRCO | BIT_CONFIG_PWR_UP | BIT_CONFIG_PRIM_RX);
		nrf_writeReg(REG_EN_AA, BIT_EN_AA_ENAA_P0);
		nrf_writeReg(REG_FEATURE, BIT_FEATURE_EN_DPL);
		nrf_writeReg(REG_DYNPD, BIT_DYNPD_DPL_P0);
		nrf_writeReg(REG_EN_RXADDR, BIT_EN_RXADDR_ERX_P0);
		writeCmd(CMD_FLUSH_TX);
		writeCmd(CMD_FLUSH_RX);
		nrf_CE_High();
	}
}

/**
 * @brief Check communication between MCU and nrf24l01p by write and read REG_TX_ADDR.
 * @param None 
 * @retval None
 */
void check_SPI(void)
{
	uint64_t addr = 0;
	writeBuff(CMD_W_REGISTER | REG_TX_ADDR, (uint8_t* )&tx_addr, 5);
	readBuff(CMD_R_REGISTER | REG_TX_ADDR, (uint8_t* )&addr, 5);
	if (tx_addr == addr)
	{
		USART_SendString(USART1, "MCU and nrf24l01p comm SUCCESS!\n");
	} else {
		USART_SendString(USART1, "MCU and nrf24l01p comm FAILED!\n");
	}
}

/**
 * @brief Send payload(for nrf_PTX_MODE).
 * @param pPayload: address of the payload that you want to send.
 * @param len: length of the payload you want to send.
 * @retval None
 */
void nrf_txPayload(uint8_t* pPayload, uint8_t len)
{
	nrf_CE_Low();
	writeBuff(CMD_W_TX_PAYLOAD, pPayload, len);
	nrf_CE_High();
}

/**
 * @brief Receive payload, return payload length.
 * @param pPayload: address where you want to store the payload.
 * @retval len: the len of received payload.
 */
uint8_t nrf_rxPayload(uint8_t* pPayload)
{
	uint8_t len;
	len = nrf_readReg(CMD_R_RX_PL_WID);
	if (0 < len && len < 33)
	{
		nrf_CE_Low();
		readBuff(CMD_R_RX_PAYLOAD, pPayload, len);
		nrf_CE_High();
	} else {
		len = 0;
	}
	writeCmd(CMD_FLUSH_RX);
	return len;
}

/**
 * @brief Send no acknowledgement payload(for nrf_PTX_MODE).
 * @param pPayload: address of the payload you want to send.
 * @param len: length of the payload you want to send.
 * @retval None
 */
void nrf_txNoAckPayload(uint8_t* pPayload, uint8_t len)
{
	while(nrf_readReg(REG_STATUS) & BIT_STATUS_TX_FULL); 	/*!< Wait until TX FIFO not full. */
	nrf_CE_Low();
	writeBuff(CMD_W_TX_PAYLOAD_NOACK, pPayload, len);
	nrf_CE_High();
}

/**
 * @brief Send acknowledgement with payload (for nrf_PRX_MODE).
 * 		Mostly acknowledgement is an empty packet(without payload),
 * 		but if you want to send data back to PTX device from PRX device,
 * 		you can send data back with acknowledgement packet as payload of
 * 		acknowledgement packet.
 * @param pPayload: address of the payload you want to send.
 * @param len: length of the payload you want to send.
 * @retval None
 */
void nrf_txAck_with_Payload(uint8_t* pPayload, uint8_t len)
{
	nrf_CE_Low();
	writeBuff(CMD_W_ACK_PAYLOAD | NRF_PIPE0, pPayload, len);
	nrf_CE_High();
}

/**
 * @brief Check events and receive a payload.
 * @param pPayload: address where you want to store the payload.
 * @param pLength: the length of received payload.
 * @retval event: events in REG_STATUS.
 */
nrf_RX_Event nrf_checkEvent_and_ReceivePayload(uint8_t* pPayload, uint8_t* pLength)
{
	nrf_RX_Event event = nrf_IDLE;
	*pLength = 0;
	uint8_t status = nrf_readReg(REG_STATUS);
	if (status & BIT_STATUS_MAX_RT)
	{
		writeCmd(CMD_FLUSH_RX);
		event = nrf_MAX_RT;
	} else if (status & BIT_STATUS_RX_DR)
	{
		*pLength = nrf_rxPayload(pPayload);
		event = nrf_RX_DR;
	} else if (status & BIT_STATUS_TX_DS)
	{
		event = nrf_TX_DS;
	}
	nrf_writeReg(REG_STATUS, BIT_STATUS_RX_DR | BIT_STATUS_TX_DS | BIT_STATUS_MAX_RT); 	/*!< Recover to reset status. */
	return event;
}

/**
 * @brief Send payload and wait for acknowledgement.
 * @param pPayload: the address of the payload you want to send.
 * @param payloadLength: the length of the payload you want to send.
 * @param pAckPayload: the address of an uint8_t array where you want to store the acknowledgement payload.
 * @param pAckPayloadLength: the length of an uint8_t array where you want to store the acknowledgement payload.
 * @retval success 1, failed 0.
 */
uint8_t nrf_sendPayload_and_WaitAck(uint8_t* pPayload, uint8_t payloadLength, uint8_t* pAckPayload, uint8_t* pAckPayloadLength)
{
	if (payloadLength == 0)
	{
		return 0;
	}
	nrf_txPayload(pPayload, payloadLength);
	while((nrf_readReg(REG_STATUS) & (BIT_STATUS_RX_DR | BIT_STATUS_TX_DS | BIT_STATUS_MAX_RT)) == 0);
	nrf_RX_Event event = nrf_checkEvent_and_ReceivePayload(pAckPayload, pAckPayloadLength);
	if (event == nrf_MAX_RT)
	{
		return 0;
	}
	return 1;
}

/**
 * @brief Standby-I mode.
 * @param None
 * @retval None
 */
void nrf_CE_Low(void)
{
	GPIO_ResetBits(NRF_CE_PORT, NRF_CE_Pin);
}

void nrf_CE_High(void)
{
	GPIO_SetBits(NRF_CE_PORT, NRF_CE_Pin);
}
