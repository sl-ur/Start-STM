// SPI.c
// Карпов Вячеслав
// 09 июня 2016

#include "SPI.h"

  // Инициализация мастера SPI1
void SPI_Init(void){
  volatile unsigned long delay;
	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN; // Активизация SPI
	RCC_APB2ENR	|= RCC_APB2ENR_IOPAEN; // Активизация порта А
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN; // Активизация альтернативной функции
	delay = RCC_APB2ENR; // 
	GPIOA_CRL &= ~0xF0FFFF00;          // Обнуление битов установки режима PA 2-5, 7
	GPIOA_CRL |= (GPIOA_CRL_MODE_PA2 | // Режим < 10 МГц
/*							GPIOA_CRL_CNF_PA2	 | // Основной двухтактный выход, 00 (push-pull) */
								GPIOA_CRL_MODE_PA3); // Режим < 10 МГц
/*							GPIOA_CRL_CNF_PA3);	 // Основной двухтактный выход, 00 (push-pull) */
	GPIOA_CRL |= (GPIOA_CRL_MODE_PA4 | // Режим < 10 МГц
								GPIOA_CRL_CNF_PA4	 | // Альтернативный двухтактный выход (push-pull) (SPI)
								GPIOA_CRL_MODE_PA5 | // Режим < 10 МГц
								GPIOA_CRL_CNF_PA5  | // Альтернативный двухтактный выход (push-pull) (SPI)
								GPIOA_CRL_MODE_PA7 | // Режим < 10 МГц
								GPIOA_CRL_CNF_PA7);  // Альтернативный двухтактный выход (push-pull) (SPI)
	SPI1_CR1 &= ~SPI1_CR1_SPE;				 // Запрет SPI1
	SPI1_CR1 &= (~SPI1_CR1_DFF &			 // Установка Формат данных 8 бит
							~SPI1_CR1_LSBFIRST &	 // Установка Передача старшего бита первым				 
							~SPI1_CR1_CPOL & 			 // Установка полряности 0
							~SPI1_CR1_CPHA); 			 // Установка фазы 0
	SPI1_CR1 |= (SPI1_CR1_BR | 				 // Установка делителя скорости передачи /16
							 SPI1_CR1_MSTR); 		   // Установка Мастер SPI						
	SPI1_CR2 |= SPI1_CR2_SSOE; 				 // Программное управление выхода NSS
	SPI1_CR1 |= SPI1_CR1_SPE;					 // Разрешение SPI1
}
