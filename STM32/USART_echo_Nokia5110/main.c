/**
* Использование имен регистров и битов, определенные через define.
* Установка/сброс битов порта посредством регистра GPIOx_BSRR.
* Использование PLL.
* USART без использования CMSIS DRIVER USART.
* USART с эхом и параллельным выводом на дисплей Nokia5110.
* Настройка только через регистры.
* Keil 5.20
* STM32F100RB
* V.0.03
* 30.05.2016
*/

#include "PLL.h"
#include "UART.h"
#include "Nokia5110.h"

int main()
{
	char buffer[256];
	PLL_Init();
	UART_Init();
	Nokia5110_Init();

	
	while(1){
		UART_InString(buffer, 256);
	}

}

