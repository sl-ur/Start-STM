// SPI.h
// Карпов Вячеслав
// 09 июня 2016

#include "def_f100x.h"

// #define RCC_APB2ENR		(*((volatile unsigned long *)0x40021018)) // Регистр тактирования USART1, SPI1 и портов ввода/вывода
#define RCC_APB2ENR_SPI1EN		0x00001000	// Разрешение тактирование SPI1
#define RCC_APB2ENR_IOPAEN		0x00000004	// Разрешение тактирования порта A
#define RCC_APB2ENR_AFIOEN		0x00000001	// Разрешение тактирования альтернативных функций входа/выхода
#define SPI1_CR1			(*((volatile unsigned long *)0x40013000)) // Управляющий регистр 1 SPI1
#define SPI1_CR2			(*((volatile unsigned long *)0x40013004)) // Управляющий регистр 2 SPI1
#define SPI1_SR				(*((volatile unsigned long *)0x40013008)) // Регистр статуса SPI1
#define SPI1_DR				(*((volatile unsigned long *)0x4001300C)) // Регистр данных SPI1
#define SPI1_CR1_DFF			    0x00000800 // Формат фрейма данных, "0" - 8 бит, "1" - 16 бит
#define SPI1_CR1_LSBFIRST	    0x00000080 // Старший бит первый - "0", младший бит первый - "1"
#define SPI1_CR1_SPE			    0x00000040 // Разрешение SPI - "1"
// #define SPI1_CR1_BR				    0x00000018 // Скорость передачи. 011 - 24/16 = 1,5 < 4
#define SPI1_CR1_BR				    0x00000010 // Скорость передачи. 010 - 24/8 = 3,0 < 4
#define SPI1_CR1_MSTR			    0x00000004 // Мастер - "1"
#define SPI1_CR1_CPOL			    0x00000002 // Полярность "1"
#define SPI1_CR1_CPHA			    0x00000001 //	Фаза "1"
#define SPI1_CR2_SSOE			    0x00000004 // Выход "Выбор ведомого" "1"
#define SPI1_SR_BSY				    0x00000800 // Флаг SPI занят - "1"
#define SPI1_SR_TXE				    0x00000002 // Флаг "Буффер пустой" - "1"
#define GPIOA_CRL_MODE_PA2	  0x00000100 // < 10 МГц
#define GPIOA_CRL_CNF_PA2			// Двухтактный выход (Reset)
#define GPIOA_CRL_MODE_PA3	  0x00001000 // < 10 МГц
#define GPIOA_CRL_CNF_PA3			// Двухтактный выход (Data)
#define GPIOA_CRL_MODE_PA4	  0x00010000 // < 10 МГц
#define GPIOA_CRL_CNF_PA4		  0x00080000 // Альтернативный двухтактный выход (SPI1_NSS)
#define GPIOA_CRL_MODE_PA5	  0x00100000 // < 10 МГц
#define GPIOA_CRL_CNF_PA5		  0x00800000 // Альтернативный двухтактный выход (SPI1_SCK)
#define GPIOA_CRL_MODE_PA7	  0x10000000 // < 10 МГц
#define GPIOA_CRL_CNF_PA7		  0x80000000 // Альтернативный двухтактный выход (SPI1_MOSI)

void SPI_Init(void);
