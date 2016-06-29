/**
* ������������� ���� ��������� � �����, ������������ ����� define.
* ���������/����� ����� ����� ����������� �������� GPIOx_BSRR.
* ������������� PLL.
* ������������� SysTick
* Keil 5.20
* STM32F100RB
* V.0.04
* 17.05.2016
*/

#include "PLL.h"
#include "SysTick.h"

// RM0041

#define RCC_CIR (*((unsigned long*) 0x40021008)) // ������� ������������ ����������
// ��������� ����� � �������� RCC_CIR
#define RCC_APB2ENR (*((unsigned long*) 0x40021018)) // ������� ��������� ������������ GPIO
// ��������� ����� � �������� RCC_APB2ENR
#define IOPAEN 0x00000004 // ���������� ������������ ����� A
#define IOPBEN 0x00000008 // ���������� ������������ ����� B
#define IOPCEN 0x00000010 // ���������� ������������ ����� C
#define IOPDEN 0x00000020 // ���������� ������������ ����� D
#define IOPEEN 0x00000040 // ���������� ������������ ����� E
#define IOPFEN 0x00000080 // ���������� ������������ ����� F
#define IOPGEN 0x00000100 // ���������� ������������ ����� G
#define ADC1EN 0x00000200 // ���������� ������������ ADC1

// �������� ������ GPIO. ���. 110.
// �������� ����� �	
#define GPIOA_CRL (*((volatile unsigned long*) 0x40010800)) // ������� ������������ �����, ���� 0 - 7
#define GPIOA_CRH (*((volatile unsigned long*) 0x40010804)) // ������� ������������ �����, ���� 0 - 15
#define GPIOA_IDR (*((volatile unsigned long*) 0x40010808)) // ������� ������� ���������� ����� A
#define GPIOA_ODR (*((volatile unsigned long*) 0x4001080C)) // ������� �������� ���������� ����� A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // ������� ���������/������ ����� ����� A
#define GPIOA_BRR (*((volatile unsigned long*) 0x40010814)) // ������� ������ ����� A
// �������� ����� �
#define GPIOC_CRL	(*((volatile unsigned long*) 0x40011000)) // ������� ������������ �����, ���� 0 - 7
#define GPIOC_CRH (*((volatile unsigned long*) 0x40011004)) // ������� ������������ �����, ���� 0 - 15
#define GPIOC_IDR (*((volatile unsigned long*) 0x40011008))
#define GPIOC_ODR (*((volatile unsigned long*) 0x4001100C))
#define GPIOC_BSRR (*((volatile unsigned long*) 0x40011010))
#define GPIOC_BRR (*((volatile unsigned long*) 0x40011014))
	
void PortA_Init(void);
void PortC_Init(void);

int main()
{
	PLL_Init();
	SysTick_Init();
	PortA_Init();
	PortC_Init();
	
	// ������� ��������� ����, PC9. ����� �������� ���, PC8. ������������� ������
	GPIOC_BSRR = 0x2000100;
	
	while(1){
		if((GPIOA_IDR & 0x1)){ // ��������� �������  ������, PA0. ������������� ������
			GPIOC_BSRR = 0x1000200; // ����� LED off. ������� LED on
			SysTick_Wait10ms(20); // �������� 200 ��
			GPIOC_BSRR = 0x3000000; // ����� LED off. ������� LED off
			SysTick_Wait10ms(20); // �������� 200 ��
		}	
		else
			GPIOC_BSRR = 0x2000100; // ����� ��������� ���. ������� �������� ����
	}
}

void PortA_Init(void)
{
	unsigned long delay;
	// 1) ��������� PortA. ������� RCC_ARB2ENR
	RCC_APB2ENR |= IOPAEN;
	// 2) �������� ��� ������������ ����������� �������� ������ ������������
		delay = RCC_APB2ENR;
	// 3) ��� ������ ������� ��� ��� ������������ ����� ���������������
	// 		� GPIO � �� ����. ������ �� ���� ����� ������. ������� GPIOA_CRL
}

void PortC_Init(void)
{
	unsigned long delay;
	// 1) ��������� PortC. ������� RCC_ARB2ENR
	RCC_APB2ENR |= IOPCEN;
	// 2) �������� ��� ������������ ����������� �������� ������ ������������
		delay = RCC_APB2ENR;
	// 3) 
	// 4) ������������� ���� PC8-9 � GPIO � �� �����. ������� GPIOC_CRH 
	GPIOC_CRH &= (~0xFF);
	GPIOC_CRH |= 0x22;
}

