/**
* ������������� ���� ��������� � �����, ������������ ����� define.
* ���������/����� ����� ����� ����������� �������� GPIOx_BSRR.
* ������������� PLL.
* USART ��� ������������� CMSIS DRIVER USART.
* USART � ����.
* ��������� ������ ����� ��������.
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

