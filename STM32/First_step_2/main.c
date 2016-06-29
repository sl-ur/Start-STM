/**
* ������������� ���� ��������� � �����, ������������ ����� define.
* Keil 5.20
* STM32F100RB
* V.0.01
* 12.05.2016
*/

// RM0041
// �������� ������ RCC - Reset Clock Control. ���. 77 
// ������� ��������� ������������ GPIO, �������� 0x18, ���. 89
#define RCC_APB2ENR *(unsigned long*) 0x40021018
// �������� ������ GPIO. ���. 110.
// �������� ����� �	
#define GPIOA_CRL (*((volatile unsigned long*) 0x40010800)) // ������� ������������ �����, ���� 0 - 7
#define GPIOA_CRH (*((volatile unsigned long*) 0x40010804)) // ������� ������������ �����, ���� 0 - 15
#define GPIOA_IDR (*((volatile unsigned long*) 0x40010808)) // ������� ������� ���������� ����� A
#define GPIOA_ODR (*((volatile unsigned long*) 0x4001080C)) // ������� �������� ���������� ����� A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // ������� ���������/������ ����� A
#define GPIOA_BRR (*((volatile unsigned long*) 0x40010814)) // ������� ������ ����� A
// �������� ����� �
#define GPIOC_CRL	(*((volatile unsigned long*) 0x40011000)) // ������� ������������ �����, ���� 0 - 7
#define GPIOC_CRH (*((volatile unsigned long*) 0x40011004)) // ������� ������������ �����, ���� 0 - 15
#define GPIOC_IDR (*((volatile unsigned long*) 0x40011008))
#define GPIOC_ODR (*((volatile unsigned long*) 0x4001100C))
#define GPIOC_BSRR (*((volatile unsigned long*) 0x40011010))
#define GPIOC_BRR (*((volatile unsigned long*) 0x40011014))

int main()
{
	unsigned long delay;
//	unsigned long Input;
//`	volatile unsigned long i = 0;
	// 1) ��������� PortA � PortC. ������� RCC_ARB2ENR
	RCC_APB2ENR |= 0x14;
	// 2) �������� ��� ������������ ����������� �������� ������ ������������
		delay = RCC_APB2ENR;
//	i++; i++; i++;
//	i = 0;
	// 3) ��� ������ ������� ��� ��� ������������ ����� ���������������
	// 		� GPIO � �� ����. ������ �� ���� ����� ������. ������� GPIOA_CRL
	// 4) ������������� ���� PC8-9 � GPIO � �� �����. ������� GPIOC_CRH 
	GPIOC_CRH &= (~0xFF);
	GPIOC_CRH |= 0x22;
	
	// ������� ��������� ����, PC9. ������������� ������
	GPIOC_ODR &= (~0x0200);
	// ����� �������� ���, PC8. ������������� ������
	GPIOC_ODR |= 0x0100;
	
	while(1){
		if((GPIOA_IDR & 0x1)){ // ��������� �������  ������, PA0. ������������� ������
			GPIOC_ODR &= (~0x0100); // ����� LED off
			GPIOC_ODR |= 0x0200; // ������� LED on
		}
		else{
			GPIOC_ODR |= 0x0100; // ����� ��������� ���
			GPIOC_ODR &= (~0x0200); // ������� �������� ����
		}
	}
}
