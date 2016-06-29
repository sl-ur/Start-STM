/**
* Использование имен регистров и битов, определенные через define.
* Keil 5.20
* STM32F100RB
* V.0.01
* 12.05.2016
*/

// RM0041
// Регистры группы RCC - Reset Clock Control. стр. 77 
// Регистр включения тактирования GPIO, смещение 0x18, стр. 89
#define RCC_APB2ENR *(unsigned long*) 0x40021018
// Регистры группы GPIO. стр. 110.
// Регистры порта А	
#define GPIOA_CRL (*((volatile unsigned long*) 0x40010800)) // Регистр конфигурации порта, пины 0 - 7
#define GPIOA_CRH (*((volatile unsigned long*) 0x40010804)) // Регистр конфигурации порта, пины 0 - 15
#define GPIOA_IDR (*((volatile unsigned long*) 0x40010808)) // Регистр входной информации порта A
#define GPIOA_ODR (*((volatile unsigned long*) 0x4001080C)) // Регистр выходной информации порта A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // Регистр установки/сброса порта A
#define GPIOA_BRR (*((volatile unsigned long*) 0x40010814)) // Регистр сброса порта A
// Регистры порта С
#define GPIOC_CRL	(*((volatile unsigned long*) 0x40011000)) // Регистр конфигурации порта, пины 0 - 7
#define GPIOC_CRH (*((volatile unsigned long*) 0x40011004)) // Регистр конфигурации порта, пины 0 - 15
#define GPIOC_IDR (*((volatile unsigned long*) 0x40011008))
#define GPIOC_ODR (*((volatile unsigned long*) 0x4001100C))
#define GPIOC_BSRR (*((volatile unsigned long*) 0x40011010))
#define GPIOC_BRR (*((volatile unsigned long*) 0x40011014))

int main()
{
	unsigned long delay;
//	unsigned long Input;
//`	volatile unsigned long i = 0;
	// 1) Тактируем PortA и PortC. Регистр RCC_ARB2ENR
	RCC_APB2ENR |= 0x14;
	// 2) Задержка для установления переходного процесса начала тактирования
		delay = RCC_APB2ENR;
//	i++; i++; i++;
//	i = 0;
	// 3) При подаче питания или при перезагрузке порты устанавливаются
	// 		в GPIO и на вход. Ничего не надо здесь делать. Регистр GPIOA_CRL
	// 4) Устанавливаем биты PC8-9 в GPIO и на выход. Регистр GPIOC_CRH 
	GPIOC_CRH &= (~0xFF);
	GPIOC_CRH |= 0x22;
	
	// Зеленый светодиод выкл, PC9. Положительная логика
	GPIOC_ODR &= (~0x0200);
	// Синий сетодиод вкл, PC8. Положительная логика
	GPIOC_ODR |= 0x0100;
	
	while(1){
		if((GPIOA_IDR & 0x1)){ // Проверяем нажатие  кнопки, PA0. Положительная логика
			GPIOC_ODR &= (~0x0100); // Синий LED off
			GPIOC_ODR |= 0x0200; // Зеленый LED on
		}
		else{
			GPIOC_ODR |= 0x0100; // Синий светодиод вкл
			GPIOC_ODR &= (~0x0200); // Зеленый сетодиод выкл
		}
	}
}
