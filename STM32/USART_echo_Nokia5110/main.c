/**
* ������������� ���� ��������� � �����, ������������ ����� define.
* ���������/����� ����� ����� ����������� �������� GPIOx_BSRR.
* ������������� PLL.
* USART ��� ������������� CMSIS DRIVER USART.
* USART � ���� � ������������ ������� �� ������� Nokia5110.
* ��������� ������ ����� ��������.
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

