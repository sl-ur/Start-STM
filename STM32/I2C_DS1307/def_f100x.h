// def_f100x.h
// ����������� ���� ��������� � ���� ����� ���������
// ���������������� ���������� STM32F100x
// ������ ��������
// 09 ���� 2016 �.

// , base adress
#define SysTick_CTRL_SR    (*((volatile unsigned long *)0xE000E010))
#define SysTick_RELOAD_VR  (*((volatile unsigned long *)0xE000E014))
#define SysTick_CURRENT_VR (*((volatile unsigned long *)0xE000E018))

// �������� RM0041
// �������� ������ RCC, base adress 0x40021000
#define RCC_CR   (*((unsigned long*) 0x40021000)) // ������� ���������� ������������
#define RCC_CFGR (*((unsigned long*) 0x40021004)) // ������� ���������������� ������������
#define RCC_CIR  (*((unsigned long*) 0x40021008)) // ������� ������������ ����������
// ��������� ����� � �������� RCC_CIR
#define RCC_APB2RSTR  (*((unsigned long*) 0x4002100C)) // 
#define RCC_APB1RSTR  (*((unsigned long*) 0x40021010)) //
// ��������� ����� � �������� RCC_APB1RSTR
#define RCC_APB1RSTR_I2C1RST 0x00200000 // ����� I2C1

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

// ������� ��������� ������������ 
#define RCC_APB1ENR (*((unsigned long*) 0x4002101C))
// ��������� ����� � �������� RCC_APB1ENR
#define RCC_APB1ENR_I2C1EN 0x00200000 // ���������� ������������ I2C1

// �������� ������ I2C1, base adress 0x40005400
#define I2C1_CR1		(*((volatile unsigned long*) 0x40005400)) // 
#define I2C1_CR2		(*((volatile unsigned long*) 0x40005404)) // 
#define I2C1_OAR1		(*((volatile unsigned long*) 0x40005408)) // 
#define I2C1_OAR2		(*((volatile unsigned long*) 0x4000540C)) // 
#define I2C1_DR			(*((volatile unsigned long*) 0x40005410)) // 
#define I2C1_SR1		(*((volatile unsigned long*) 0x40005414)) // 
#define I2C1_SR2		(*((volatile unsigned long*) 0x40005418)) // 
#define I2C1_CCR		(*((volatile unsigned long*) 0x4000541C)) // 
#define I2C1_TRISE	(*((volatile unsigned long*) 0x40005420)) // 
// �������� ����� ��������� I2C1
// #define 


// �������� ������ GPIO. ���. 110.
// �������� ����� �, base adress 0x40010800	
#define GPIOA_CRL  (*((volatile unsigned long*) 0x40010800)) // ������� ������������ �����, PA[7:0]
#define GPIOA_CRH  (*((volatile unsigned long*) 0x40010804)) // ������� ������������ �����, PA[15:8]
#define GPIOA_IDR  (*((volatile unsigned long*) 0x40010808)) // ������� ������� ���������� ����� A
#define GPIOA_ODR  (*((volatile unsigned long*) 0x4001080C)) // ������� �������� ���������� ����� A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // ������� ���������/������ ����� ����� A
#define GPIOA_BRR  (*((volatile unsigned long*) 0x40010814)) // ������� ������ ����� ����� A
// �������� ����� B, base adress 0x40010C00
#define GPIOB_CRL	 (*((volatile unsigned long*) 0x40010C00)) // ������� ������������ �����, PB[7:0]
#define GPIOB_CRH  (*((volatile unsigned long*) 0x40010C04)) // ������� ������������ �����, PB[15:8]
#define GPIOB_IDR  (*((volatile unsigned long*) 0x40010C08)) // ������� ������� ���������� ����� B
#define GPIOB_ODR  (*((volatile unsigned long*) 0x40010C0C)) // ������� �������� ���������� ����� B
#define GPIOB_BSRR (*((volatile unsigned long*) 0x40010C10)) // ������� ���������/������ ����� ����� B
#define GPIOB_BRR  (*((volatile unsigned long*) 0x40010C14)) // ������� ������ ����� ����� B  
// �������� ����� �, base adress 0x40011000
#define GPIOC_CRL	 (*((volatile unsigned long*) 0x40011000)) // ������� ������������ �����, PC[7:0]
#define GPIOC_CRH  (*((volatile unsigned long*) 0x40011004)) // ������� ������������ �����, PC[15:8]
#define GPIOC_IDR  (*((volatile unsigned long*) 0x40011008)) // ������� ������� ���������� ����� C
#define GPIOC_ODR  (*((volatile unsigned long*) 0x4001100C)) // ������� �������� ���������� ����� C
#define GPIOC_BSRR (*((volatile unsigned long*) 0x40011010)) // ������� ���������/������ ����� ����� C
#define GPIOC_BRR  (*((volatile unsigned long*) 0x40011014)) // ������� ������ ����� ����� C
 
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
  
// �������� ������ SPI, base adress 
#define SPI1_CR1			(*((volatile unsigned long *)0x40013000)) // ����������� ������� 1 SPI1
#define SPI1_CR2			(*((volatile unsigned long *)0x40013004)) // ����������� ������� 2 SPI1
#define SPI1_SR				(*((volatile unsigned long *)0x40013008)) // ������� ������� SPI1
#define SPI1_DR				(*((volatile unsigned long *)0x4001300C)) // ������� ������ SPI1
  
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

