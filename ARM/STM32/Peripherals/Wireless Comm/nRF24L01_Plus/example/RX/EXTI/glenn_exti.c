/**
 ******************************************************************************
 * @file 		glenn_exti.c
 * @author 		Glenn Pallad
 * @version 	V0.8.0_Alpha
 * @date 		August 30, 2018
 * @brief 		This file is EXTI driver.
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

#include "glenn_exti.h"

/**
 * @brief This is a setting example, where 
 * 		GPIOA Pin 0 were attached to EXTI_Line0, 
 * 		GPIOC Pin 13 were attached to EXTI_Line13, 
 * 		modify this to meet your needs.
 *		Maximum 16(0..15）EXTIs supported simultaneously.
 *		Use
 * 			EXTI_GetITStatus(EXTI_Linex)
 *		in Handlers to detect which EXTI_Line was triggered.
 * 		Use
 *			EXTI_ClearITPendingBit(EXTI_Linex)
 *		in Handlers to Clear EXTI_Line bit when Handler is finished.
 *		where x can be (0..15).
 */
void EXTI_Configuration(void){
	EXTI_InitTypeDef EXTI_InitStructure;
	/*!< EXTI for NRF 
		GPIOA Pin 1 NRF_IRQ */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
