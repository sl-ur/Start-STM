/**
* Прерывания. INT.
* Нажатие кнопки, порт PA0, вызывает прерывание по переднему фронту. 
* Обработчик прерывания подсчитывает количество нажатий на кнопку. 
* Количестово нажатий отображается на дисплее Nokia5110.
* Прерывание без приоритета.
* Использование имен регистров и битов, определенные через define.
* Установка/сброс битов порта посредством регистра GPIOx_BSRR.
* Использование PLL.
* Настройка только через регистры.
* Keil 5.20
* STM32F100RB
* V.0.01
* 02.06.2016
*/

#include "PLL.h"
#include "SysTick.h"
#include "Nokia5110.h"

// RM0041

#define RCC_CIR (*((unsigned long*) 0x40021008)) // Регистр тактирования прерываний
// Установка битов в регистре RCC_CIR
#define RCC_APB2ENR (*((unsigned long*) 0x40021018)) // Регистр включения тактирования GPIO
// Установка битов в регистре RCC_APB2ENR
#define RCC_APB2ENR_IOPAEN 0x00000004 // Разрешение тактирования порта A
#define RCC_APB2ENR_IOPBEN 0x00000008 // Разрешение тактирования порта B
#define RCC_APB2ENR_IOPCEN 0x00000010 // Разрешение тактирования порта C
#define RCC_APB2ENR_IOPDEN 0x00000020 // Разрешение тактирования порта D
#define RCC_APB2ENR_IOPEEN 0x00000040 // Разрешение тактирования порта E
#define RCC_APB2ENR_IOPFEN 0x00000080 // Разрешение тактирования порта F
#define RCC_APB2ENR_IOPGEN 0x00000100 // Разрешение тактирования порта G
#define RCC_APB2ENR_ADC1EN 0x00000200 // Разрешение тактирования ADC1
#define RCC_APB2ENR_AFIOEN 0x00000001 // Разрешение тактирования AFIO

// Регистры группы GPIO. стр. 110.
// Регистры порта А	
#define GPIOA_CRL (*((volatile unsigned long*) 0x40010800)) // Регистр конфигурации порта, PA[7:0]
#define GPIOA_CRH (*((volatile unsigned long*) 0x40010804)) // Регистр конфигурации порта, PA[15:8]
#define GPIOA_IDR (*((volatile unsigned long*) 0x40010808)) // Регистр входной информации порта A
#define GPIOA_ODR (*((volatile unsigned long*) 0x4001080C)) // Регистр выходной информации порта A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // Регистр установки/сброса битов порта A
#define GPIOA_BRR (*((volatile unsigned long*) 0x40010814)) // Регистр сброса битов порта A
// Регистры порта С
#define GPIOC_CRL	(*((volatile unsigned long*) 0x40011000)) // Регистр конфигурации порта, PC[7:0]
#define GPIOC_CRH (*((volatile unsigned long*) 0x40011004)) // Регистр конфигурации порта, PC[15:8]
#define GPIOC_IDR (*((volatile unsigned long*) 0x40011008)) // Регистр входной информации порта C
#define GPIOC_ODR (*((volatile unsigned long*) 0x4001100C)) // Регистр выходной информации порта C
#define GPIOC_BSRR (*((volatile unsigned long*) 0x40011010)) // Регистр установки/сброса битов порта C
#define GPIOC_BRR (*((volatile unsigned long*) 0x40011014)) // Регистр сброса битов порта C
 
// Регистры группы AFIO
#define AFIO_EXTICR1 (*((volatile unsigned long*) 0x40010008)) // 
#define AFIO_EXTICR1_PA0 0x0000 // 
#define AFIO_EXTICR1_PB0 0X0001 // 
// Регистры группы EXTI
#define EXTI_IMR    (*((volatile unsigned long*) 0x40010400)) // Установка маскируемых прерываний
#define EXTI_EMR    (*((volatile unsigned long*) 0x40010404)) // Установка маскируемых событий
#define EXTI_RTSR   (*((volatile unsigned long*) 0x40010408)) // Прерывание по фронту
#define EXTI_FRSR   (*((volatile unsigned long*) 0x4001040C)) // Прерывание по спаду
#define EXTI_SWIER  (*((volatile unsigned long*) 0x40010410)) // Программное прерывание
#define EXTI_PR     (*((volatile unsigned long*) 0x40010414)) // Установка задержки прерываний
// Регистры группы NVIC, base adress 0xE000E100
#define NVIC_ISER0  (*((volatile unsigned long*) 0xE000E100)) // Разрешение прерываний IRQ[31:0]
#define NVIC_ISER1  (*((volatile unsigned long*) 0xE000E104)) // Разрешение прерываний IRQ[63:32]
#define NVIC_ISER2  (*((volatile unsigned long*) 0xE000E108)) // Разрешение прерываний IRQ[80:64]
#define NVIC_ICER0	(*((volatile unsigned long*) 0xE000E180)) // Запрет прерываний IRQ[31:0]
#define NVIC_ICER1	(*((volatile unsigned long*) 0xE000E184)) // Запрет прерываний IRQ[63:32]
#define NVIC_ICER2	(*((volatile unsigned long*) 0xE000E188)) // Запрет прерываний IRQ[80:64]
#define NVIC_ISPR0  (*((volatile unsigned long*) 0xE000E200)) // [31:0]
#define NVIC_ISPR1  (*((volatile unsigned long*) 0xE000E204)) // [63:32]
#define NVIC_ISPR2  (*((volatile unsigned long*) 0xE000E208)) // [80:64]
	
 void PortA_Init(void);
 void PortC_Init(void);

volatile unsigned long NamberRisingEdges = 0;
void EXTI0_IRQHandler(void){
	SysTick_Wait10ms(20); // задержка 200 мс от дребезга контактов кнопки
  EXTI_PR |= 0x00000001; // 
  NamberRisingEdges++;
}

int main()
{
  volatile unsigned long oldNamberRisingEdges = 0;
	
	PLL_Init();
	PortA_Init();
	PortC_Init();
	SysTick_Init();
	Nokia5110_Init();
	
// Зеленый светодиод выкл, PC9. Синий светодиод вкл, PC8. Положительная логика
	GPIOC_BSRR = 0x2000100;
  Nokia5110_Clear();	Nokia5110_SetCursor(0, 0);
  Nokia5110_OutString("Interrupts");
  Nokia5110_SetCursor(0, 1);
  Nokia5110_OutString("Number: ");
	
	while(1){
    if(oldNamberRisingEdges != NamberRisingEdges){
      if(NamberRisingEdges > 0xFF) 
        NamberRisingEdges = 0;
      oldNamberRisingEdges = NamberRisingEdges;
			Nokia5110_SetCursor(7, 1);
			Nokia5110_OutUDec(oldNamberRisingEdges);
    }  
    
    
/*		if((GPIOA_IDR & 0x1)){ // Проверяем нажатие  кнопки, PA0. Положительная логика
			GPIOC_BSRR = 0x1000200; // Синий LED off. Зеленый LED on
			SysTick_Wait10ms(20); // задержка 200 мс
			GPIOC_BSRR = 0x3000000; // Синий LED off. Зеленый LED off
			SysTick_Wait10ms(20); // задержка 200 мс
			Nokia5110_OutString("Hello,World!");
		}	
		else
			GPIOC_BSRR = 0x2000100; // Синий светодиод вкл. Зеленый светодиод выкл
*/
	}
}

void PortA_Init(void)
{
	unsigned long delay;
	// 1) Тактируем PortA. Регистр RCC_ARB2ENR
	RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
	// 2) Задержка для установления переходного процесса начала тактирования
	delay = RCC_APB2ENR;
	// 3) При подаче питания или при перезагрузке порты устанавливаются
	// 		в GPIO и на вход. Ничего не надо здесь делать. Регистр GPIOA_CRL
  // Инициализация прерывания порта PA0
	NVIC_ICER0 |= 0x00000040;
  AFIO_EXTICR1 &= ~0x0000000F; 			// Очистка битов 
	AFIO_EXTICR1 |= AFIO_EXTICR1_PA0; // a) Для порта PA0 EXTI0(3:0) - b0000
  EXTI_IMR |= 0x00000001;      			// b) 
  EXTI_RTSR |= 0x00000001;     			// c) 
	EXTI_PR |= 0x00000001;			 			// d)
	// Инициализация NVIC
	NVIC_ISER0 |= 0x00000040;
  
}

void PortC_Init(void)
{
	unsigned long delay;
	// 1) Тактируем PortC. Регистр RCC_ARB2ENR
	RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;
	// 2) Задержка для установления переходного процесса начала тактирования
		delay = RCC_APB2ENR;
	// 3) 
	// 4) Устанавливаем биты PC8-9 в GPIO и на выход. Регистр GPIOC_CRH 
	GPIOC_CRH &= (~0xFF);
	GPIOC_CRH |= 0x22;
}

