# PWM Example
This is an example showing how to set up PWM in STM32F10x HD devices.

In this example, **GPIOB Pin 0** was used as the output of PWM, there are 4 pre-defined PWM levels in *stm32f10x_it.c* file, the variable `pwmIndex` was used to store the value of PWM levels which can range from 0 to 1023.

# Usage
Link **GPIOA Pin 9** and **GPIOA Pin 10** of your MCU as TX and RX of UART to a computer which has a **Serial Debugging Software** already installed, select the level you want by sending chars `'0'`, `'1'`, `'2'` or `'4'` (without `CR`(return) or `LF`(End-of-line))from the **Serial Debugging Software** in your computer to MCU, you should get PWM output from MCU's **GPIOB Pin 0**.