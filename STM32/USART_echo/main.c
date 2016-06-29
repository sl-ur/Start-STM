/**
* Использование имен регистров и битов, определенные через define.
* Установка/сброс битов порта посредством регистра GPIOx_BSRR.
* Использование PLL.
* USART без использования CMSIS DRIVER USART.
* USART с эхом.
* Настройка только через регистры.
* Keil 5.20
* STM32F100RB
* V.0.02
* 30.05.2016
*/

#include "PLL.h"
#include "UART.h"

int main()
{
	char buffer[256];
	PLL_Init();
	UART_Init();

	
	while(1){

			UART_InString(buffer, 256);
	}

}

