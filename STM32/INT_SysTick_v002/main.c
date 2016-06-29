/**
* Прерывания. INT.
* Прерывание от SysTick. Генерирует 1 кГц. 
* Выход меандра на порт PC7. Подключения наушника для прослушивания колебаний. 
* Прерывание без приоритета.
* Использование имен регистров и битов, определенные через define.
* Использование PLL, 24 МГц.
* Кварц 8 МГц.
* Настройка только через регистры.
* Keil 5.20
* STM32F100RB
* V.0.02 Определения перенесены в файл def_f100x.h
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
	
// Зеленый светодиод выкл, PC9. Синий светодиод вкл, PC8. Положительная логика
	GPIOC_BSRR = 0x2000100;
	GPIOC_ODR |= 0x80;
	
	while(1){
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
  // Бит PC7 на выход. Регистр GPIOC_CRL
  GPIOC_CRL &=(~0xF0000000);
  GPIOC_CRL |= 0x20000000;
}

