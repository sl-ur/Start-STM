/**
* ����������. INT.
* ���������� �� SysTick. ���������� 1 ���. 
* ����� ������� �� ���� PC7. ����������� �������� ��� ������������� ���������. 
* ���������� ��� ����������.
* ������������� ���� ��������� � �����, ������������ ����� define.
* ������������� PLL, 24 ���.
* ����� 8 ���.
* ��������� ������ ����� ��������.
* Keil 5.20
* STM32F100RB
* V.0.02 ����������� ���������� � ���� def_f100x.h
* 09.06.2016
*/

#include "PLL.h"
#include "INT_SysTick.h"
#include "def_f100x.h"
	
 void PortC_Init(void);

void SysTick_Handler(void){
  GPIOC_ODR ^= 0x80;
}

int main()
{	
	PLL_Init();
	PortC_Init();
	INT_SysTick_Init();
	
// ������� ��������� ����, PC9. ����� ��������� ���, PC8. ������������� ������
	GPIOC_BSRR = 0x2000100;
	GPIOC_ODR |= 0x80;
	
	while(1){
	}
}

void PortA_Init(void)
{
	unsigned long delay;
	// 1) ��������� PortA. ������� RCC_ARB2ENR
	RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
	// 2) �������� ��� ������������ ����������� �������� ������ ������������
	delay = RCC_APB2ENR;
	// 3) ��� ������ ������� ��� ��� ������������ ����� ���������������
	// 		� GPIO � �� ����. ������ �� ���� ����� ������. ������� GPIOA_CRL
  // ������������� ���������� ����� PA0
	NVIC_ICER0 |= 0x00000040;
  AFIO_EXTICR1 &= ~0x0000000F; 			// ������� ����� 
	AFIO_EXTICR1 |= AFIO_EXTICR1_PA0; // a) ��� ����� PA0 EXTI0(3:0) - b0000
  EXTI_IMR |= 0x00000001;      			// b) 
  EXTI_RTSR |= 0x00000001;     			// c) 
	EXTI_PR |= 0x00000001;			 			// d)
	// ������������� NVIC
	NVIC_ISER0 |= 0x00000040;
  
}

void PortC_Init(void)
{
	unsigned long delay;
	// 1) ��������� PortC. ������� RCC_ARB2ENR
	RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;
	// 2) �������� ��� ������������ ����������� �������� ������ ������������
		delay = RCC_APB2ENR;
	// 3) 
	// 4) ������������� ���� PC8-9 � GPIO � �� �����. ������� GPIOC_CRH 
	GPIOC_CRH &= (~0xFF);
	GPIOC_CRH |= 0x22;
  // ��� PC7 �� �����. ������� GPIOC_CRL
  GPIOC_CRL &=(~0xF0000000);
  GPIOC_CRL |= 0x20000000;
}

