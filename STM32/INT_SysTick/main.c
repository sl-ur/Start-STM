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
* V.0.01
* 05.06.2016
*/

#include "PLL.h"
#include "INT_SysTick.h"

// RM0041

// , base adress 
#define RCC_CIR (*((unsigned long*) 0x40021008)) // ������� ������������ ����������
// ��������� ����� � �������� RCC_CIR
#define RCC_APB2ENR (*((unsigned long*) 0x40021018)) // ������� ��������� ������������ GPIO
// ��������� ����� � �������� RCC_APB2ENR
#define RCC_APB2ENR_IOPAEN 0x00000004 // ���������� ������������ ����� A
#define RCC_APB2ENR_IOPBEN 0x00000008 // ���������� ������������ ����� B
#define RCC_APB2ENR_IOPCEN 0x00000010 // ���������� ������������ ����� C
#define RCC_APB2ENR_IOPDEN 0x00000020 // ���������� ������������ ����� D
#define RCC_APB2ENR_IOPEEN 0x00000040 // ���������� ������������ ����� E
#define RCC_APB2ENR_IOPFEN 0x00000080 // ���������� ������������ ����� F
#define RCC_APB2ENR_IOPGEN 0x00000100 // ���������� ������������ ����� G
#define RCC_APB2ENR_ADC1EN 0x00000200 // ���������� ������������ ADC1
#define RCC_APB2ENR_AFIOEN 0x00000001 // ���������� ������������ AFIO

// �������� ������ GPIO. ���. 110.
// �������� ����� �, base adress 	
#define GPIOA_CRL (*((volatile unsigned long*) 0x40010800)) // ������� ������������ �����, PA[7:0]
#define GPIOA_CRH (*((volatile unsigned long*) 0x40010804)) // ������� ������������ �����, PA[15:8]
#define GPIOA_IDR (*((volatile unsigned long*) 0x40010808)) // ������� ������� ���������� ����� A
#define GPIOA_ODR (*((volatile unsigned long*) 0x4001080C)) // ������� �������� ���������� ����� A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // ������� ���������/������ ����� ����� A
#define GPIOA_BRR (*((volatile unsigned long*) 0x40010814)) // ������� ������ ����� ����� A
// �������� ����� �, base adress 
#define GPIOC_CRL	(*((volatile unsigned long*) 0x40011000)) // ������� ������������ �����, PC[7:0]
#define GPIOC_CRH (*((volatile unsigned long*) 0x40011004)) // ������� ������������ �����, PC[15:8]
#define GPIOC_IDR (*((volatile unsigned long*) 0x40011008)) // ������� ������� ���������� ����� C
#define GPIOC_ODR (*((volatile unsigned long*) 0x4001100C)) // ������� �������� ���������� ����� C
#define GPIOC_BSRR (*((volatile unsigned long*) 0x40011010)) // ������� ���������/������ ����� ����� C
#define GPIOC_BRR (*((volatile unsigned long*) 0x40011014)) // ������� ������ ����� ����� C
 
// �������� ������ AFIO, base adress 
#define AFIO_EXTICR1 (*((volatile unsigned long*) 0x40010008)) // 
#define AFIO_EXTICR1_PA0 0x0000 // 
#define AFIO_EXTICR1_PB0 0X0001 // 
// �������� ������ EXTI, base adress 
#define EXTI_IMR    (*((volatile unsigned long*) 0x40010400)) // ��������� ����������� ����������
#define EXTI_EMR    (*((volatile unsigned long*) 0x40010404)) // ��������� ����������� �������
#define EXTI_RTSR   (*((volatile unsigned long*) 0x40010408)) // ���������� �� ������
#define EXTI_FRSR   (*((volatile unsigned long*) 0x4001040C)) // ���������� �� �����
#define EXTI_SWIER  (*((volatile unsigned long*) 0x40010410)) // ����������� ����������
#define EXTI_PR     (*((volatile unsigned long*) 0x40010414)) // ��������� �������� ����������
// �������� ������ NVIC, base adress 0xE000E100
#define NVIC_ISER0  (*((volatile unsigned long*) 0xE000E100)) // ���������� ���������� IRQ[31:0]
#define NVIC_ISER1  (*((volatile unsigned long*) 0xE000E104)) // ���������� ���������� IRQ[63:32]
#define NVIC_ISER2  (*((volatile unsigned long*) 0xE000E108)) // ���������� ���������� IRQ[80:64]
#define NVIC_ICER0	(*((volatile unsigned long*) 0xE000E180)) // ������ ���������� IRQ[31:0]
#define NVIC_ICER1	(*((volatile unsigned long*) 0xE000E184)) // ������ ���������� IRQ[63:32]
#define NVIC_ICER2	(*((volatile unsigned long*) 0xE000E188)) // ������ ���������� IRQ[80:64]
#define NVIC_ISPR0  (*((volatile unsigned long*) 0xE000E200)) // ��������� �������� ���������� IRQ[31:0]
#define NVIC_ISPR1  (*((volatile unsigned long*) 0xE000E204)) // ��������� �������� ���������� IRQ[63:32]
#define NVIC_ISPR2  (*((volatile unsigned long*) 0xE000E208)) // ��������� �������� ���������� IRQ[80:64]
	
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

