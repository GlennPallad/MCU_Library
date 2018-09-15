/**
 ******************************************************************************
 * @file 		num_to_ascii.h
 * @author 		Glenn Pallad
 * @version 	V0.8.0_Alpha
 * @date 		August 25, 2018
 * @brief 		This file provides all functions for converting numbers to 
 * 				chars.
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

#include "num_to_ascii.h"

/**
 * @brief Convert a uint32_t type number to ASCII string in binary form.
 * @param number: a uint32_t type number, for example b1010 1010 1010 1010 1010 1010 1010 1010.
 * @param str: a pointer to a char array MUST at size of 36(32 + '0', 'b', '\n' and '\0'),
 * 		since this is a pointer, the parameter will be changed directly, so there is no need
 * 		to return a value.
 * @param sizeof_str: sizeof(str).
 * @retval None
 */
void numberToASCIIBin(uint32_t number, char * str, int sizeof_str)
{
	
	if (sizeof_str != 36)
	{
		#ifdef GCC_PLATFORMS
			printf("Error! The size of str is not 36.\n");
			return;
		#endif
		return;
	}

	uint32_t andBit = 0b10000000000000000000000000000000;
	uint32_t currBit;
	int bitVal;
	str[0] = '0';
	str[1] = 'b';
	str[34] = '\n';
	str[35] = '\0';
	for (int i = 2; i < 34; ++i)
	{
		// And out bit by bit from MSB to LSB.
		currBit = number & andBit;
		bitVal = (currBit >> (33 - i));
		str[i] = bitVal + ASCII_OFFSET;
		andBit = andBit >> 1;
	}
	return;
}

// void numberToASCIIDec(uint32_t number, char * str, int sizeof_str){
//     return '\n';
// }

// void numberToASCIIHex(uint32_t number, char * str, int sizeof_str);
//     return '\n';
// }
