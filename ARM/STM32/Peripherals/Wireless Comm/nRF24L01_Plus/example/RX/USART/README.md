# Example
## Send String
Use `void USART_SendString(USART_TypeDef* USARTx, char* str)` to send a string.
Note that this function only works at 8-bit mode(7 bits data + 1 bit parity), and this 
function will occupies the processor until transmission finished.<br>
*main.c*
```C
#include "stm32f10x.h"
#include "glenn_usart.h"

int main(int argc, char const *argv[])
{
	char welcome[] = "Welcome to USART1\n";
	USART_Configuration();
	USART_SendString(USART1, welcome);

	return 0;
}
```
<br>

## Receive Data
Handle received data by USART1 interrupt.
Use `uint16_t USART_ReceiveData(USART_TypeDef* USARTx)` from STM official libraries
to receive 1 frame data.<br>
*stm32f10x_it.c*
```C
#include "stm32f10x_it.h"
#include "glenn_usart.h"

void USART1_IRQHandler(void){
	uint16_t temp;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != 0)
	{
		temp = USART_ReceiveData(USART1);
		USART_SendData(USART1, temp);
	}
}
```