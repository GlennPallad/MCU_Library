# Example Illustration
This is an example for nrf24l01+(includes TX and RX).

There is a key button **KEY1** connects to MCU in PTX side, once the key was pressed, an interrupt will be triggered, the pre-defined messages will be sent to PRX from PTX.

NOTE: LED and FreeRTOS codes were not provided, but this doesn't affect transmission functions. <br>
LED codes are very simple, you can write LED codes according to your PCB by yourselves.<br>
Download FreeRTOS codes from its [web site](www.freertos.org), and here is how to [Creating a New FreeRTOS Project from Scratch](http://blog.leanote.com/post/glennpallad/fa0dbe25ed28).

# Hookups
## TX
| MCU | nRF24L01+ |
| ------ | ------ |
| GPIOC5 | CE |
| GPIOC6 | NSS |
| GPIOA5 | SCK |
| GPIOA7 | MOSI |
| GPIOA6 | MISO |
| GPIOC4 | IRQ |

GPIOA0 ----- KEY1

## RX
| MCU | nRF24L01+ |
| ------ | ------ |
| GPIOA2 | CE |
| GPIOA4 | NSS |
| GPIOA5 | SCK |
| GPIOA7 | MOSI |
| GPIOA6 | MISO |
| GPIOA1 | IRQ |

# Test Result
`"Hi, This is GP!"` was sent from PTX and received by PRX successfully.<br>
![test result](https://image.ibb.co/dmEbEp/TIM_20180911213332.png)
