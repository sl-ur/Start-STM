/**
* Использование имен регистров и битов, определенные через define.
* Установка/сброс битов порта посредством регистра GPIOx_BSRR.
* Использование PLL.
* Использование SysTick
* Keil 5.20
* STM32F100RB
* V.0.04
* 17.05.2016
*/

#include "PLL.h"
#include "SysTick.h"

// RM0041

#define RCC_CIR (*((unsigned long*) 0x40021008)) // Регистр тактирования прерываний
// Установка битов в регистре RCC_CIR
#define RCC_APB2ENR (*((unsigned long*) 0x40021018)) // Регистр включения тактирования GPIO
// Установка битов в регистре RCC_APB2ENR
#define IOPAEN 0x00000004 // Разрешение тактирования порта A
#define IOPBEN 0x00000008 // Разрешение тактирования порта B
#define IOPCEN 0x00000010 // Разрешение тактирования порта C
#define IOPDEN 0x00000020 // Разрешение тактирования порта D
#define IOPEEN 0x00000040 // Разрешение тактирования порта E
#define IOPFEN 0x00000080 // Разрешение тактирования порта F
#define IOPGEN 0x00000100 // Разрешение тактирования порта G
#define ADC1EN 0x00000200 // Разрешение тактирования ADC1

// Регистры группы GPIO. стр. 110.
// Регистры порта А	
#define GPIOA_CRL (*((volatile unsigned long*) 0x40010800)) // Регистр конфигурации порта, пины 0 - 7
#define GPIOA_CRH (*((volatile unsigned long*) 0x40010804)) // Регистр конфигурации порта, пины 0 - 15
#define GPIOA_IDR (*((volatile unsigned long*) 0x40010808)) // Регистр входной информации порта A
#define GPIOA_ODR (*((volatile unsigned long*) 0x4001080C)) // Регистр выходной информации порта A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // Регистр установки/сброса битов порта A
#define GPIOA_BRR (*((volatile unsigned long*) 0x40010814)) // Регистр сброса порта A
// Регистры порта С
#define GPIOC_CRL	(*((volatile unsigned long*) 0x40011000)) // Регистр конфигурации порта, пины 0 - 7
#define GPIOC_CRH (*((volatile unsigned long*) 0x40011004)) // Регистр конфигурации порта, пины 0 - 15
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
	
	// Зеленый светодиод выкл, PC9. Синий сетодиод вкл, PC8. Положительная логика
	GPIOC_BSRR = 0x2000100;
	
	while(1){
		if((GPIOA_IDR & 0x1)){ // Проверяем нажатие  кнопки, PA0. Положительная логика
			GPIOC_BSRR = 0x1000200; // Синий LED off. Зеленый LED on
			SysTick_Wait10ms(20); // задержка 200 мс
			GPIOC_BSRR = 0x3000000; // Синий LED off. Зеленый LED off
			SysTick_Wait10ms(20); // задержка 200 мс
		}	
		else
			GPIOC_BSRR = 0x2000100; // Синий светодиод вкл. Зеленый сетодиод выкл
	}
}

void PortA_Init(void)
{
	unsigned long delay;
	// 1) Тактируем PortA. Регистр RCC_ARB2ENR
	RCC_APB2ENR |= IOPAEN;
	// 2) Задержка для установления переходного процесса начала тактирования
		delay = RCC_APB2ENR;
	// 3) При подаче питания или при перезагрузке порты устанавливаются
	// 		в GPIO и на вход. Ничего не надо здесь делать. Регистр GPIOA_CRL
}

void PortC_Init(void)
{
	unsigned long delay;
	// 1) Тактируем PortC. Регистр RCC_ARB2ENR
	RCC_APB2ENR |= IOPCEN;
	// 2) Задержка для установления переходного процесса начала тактирования
		delay = RCC_APB2ENR;
	// 3) 
	// 4) Устанавливаем биты PC8-9 в GPIO и на выход. Регистр GPIOC_CRH 
	GPIOC_CRH &= (~0xFF);
	GPIOC_CRH |= 0x22;
}

