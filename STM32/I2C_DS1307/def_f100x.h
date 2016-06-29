// def_f100x.h
// Определение имен регистров и имен битов регистров
// микроконтроллера сеймейства STM32F100x
// Карпов Вячеслав
// 09 июня 2016 г.

// , base adress
#define SysTick_CTRL_SR    (*((volatile unsigned long *)0xE000E010))
#define SysTick_RELOAD_VR  (*((volatile unsigned long *)0xE000E014))
#define SysTick_CURRENT_VR (*((volatile unsigned long *)0xE000E018))

// Документ RM0041
// Регистры группы RCC, base adress 0x40021000
#define RCC_CR   (*((unsigned long*) 0x40021000)) // Регистр управления тактирования
#define RCC_CFGR (*((unsigned long*) 0x40021004)) // Регистр конфигурирования тактирования
#define RCC_CIR  (*((unsigned long*) 0x40021008)) // Регистр тактирования прерываний
// Установка битов в регистре RCC_CIR
#define RCC_APB2RSTR  (*((unsigned long*) 0x4002100C)) // 
#define RCC_APB1RSTR  (*((unsigned long*) 0x40021010)) //
// Установка битов в регистре RCC_APB1RSTR
#define RCC_APB1RSTR_I2C1RST 0x00200000 // Сброс I2C1

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

// Регистр включения тактирования 
#define RCC_APB1ENR (*((unsigned long*) 0x4002101C))
// Установка битов в регистре RCC_APB1ENR
#define RCC_APB1ENR_I2C1EN 0x00200000 // Разрешение тактирования I2C1

// Регистры группы I2C1, base adress 0x40005400
#define I2C1_CR1		(*((volatile unsigned long*) 0x40005400)) // 
#define I2C1_CR2		(*((volatile unsigned long*) 0x40005404)) // 
#define I2C1_OAR1		(*((volatile unsigned long*) 0x40005408)) // 
#define I2C1_OAR2		(*((volatile unsigned long*) 0x4000540C)) // 
#define I2C1_DR			(*((volatile unsigned long*) 0x40005410)) // 
#define I2C1_SR1		(*((volatile unsigned long*) 0x40005414)) // 
#define I2C1_SR2		(*((volatile unsigned long*) 0x40005418)) // 
#define I2C1_CCR		(*((volatile unsigned long*) 0x4000541C)) // 
#define I2C1_TRISE	(*((volatile unsigned long*) 0x40005420)) // 
// Установк битов регистров I2C1
// #define 


// Регистры группы GPIO. стр. 110.
// Регистры порта А, base adress 0x40010800	
#define GPIOA_CRL  (*((volatile unsigned long*) 0x40010800)) // Регистр конфигурации порта, PA[7:0]
#define GPIOA_CRH  (*((volatile unsigned long*) 0x40010804)) // Регистр конфигурации порта, PA[15:8]
#define GPIOA_IDR  (*((volatile unsigned long*) 0x40010808)) // Регистр входной информации порта A
#define GPIOA_ODR  (*((volatile unsigned long*) 0x4001080C)) // Регистр выходной информации порта A
#define GPIOA_BSRR (*((volatile unsigned long*) 0x40010810)) // Регистр установки/сброса битов порта A
#define GPIOA_BRR  (*((volatile unsigned long*) 0x40010814)) // Регистр сброса битов порта A
// Регистры порта B, base adress 0x40010C00
#define GPIOB_CRL	 (*((volatile unsigned long*) 0x40010C00)) // Регистр конфигурации порта, PB[7:0]
#define GPIOB_CRH  (*((volatile unsigned long*) 0x40010C04)) // Регистр конфигурации порта, PB[15:8]
#define GPIOB_IDR  (*((volatile unsigned long*) 0x40010C08)) // Регистр входной информации порта B
#define GPIOB_ODR  (*((volatile unsigned long*) 0x40010C0C)) // Регистр выходной информации порта B
#define GPIOB_BSRR (*((volatile unsigned long*) 0x40010C10)) // Регистр установки/сброса битов порта B
#define GPIOB_BRR  (*((volatile unsigned long*) 0x40010C14)) // Регистр сброса битов порта B  
// Регистры порта С, base adress 0x40011000
#define GPIOC_CRL	 (*((volatile unsigned long*) 0x40011000)) // Регистр конфигурации порта, PC[7:0]
#define GPIOC_CRH  (*((volatile unsigned long*) 0x40011004)) // Регистр конфигурации порта, PC[15:8]
#define GPIOC_IDR  (*((volatile unsigned long*) 0x40011008)) // Регистр входной информации порта C
#define GPIOC_ODR  (*((volatile unsigned long*) 0x4001100C)) // Регистр выходной информации порта C
#define GPIOC_BSRR (*((volatile unsigned long*) 0x40011010)) // Регистр установки/сброса битов порта C
#define GPIOC_BRR  (*((volatile unsigned long*) 0x40011014)) // Регистр сброса битов порта C
 
// Регистры группы AFIO, base adress 
#define AFIO_EXTICR1 (*((volatile unsigned long*) 0x40010008)) // 
#define AFIO_EXTICR1_PA0 0x0000 // 
#define AFIO_EXTICR1_PB0 0X0001 // 

// Регистры группы EXTI, base adress 
#define EXTI_IMR    (*((volatile unsigned long*) 0x40010400)) // Установка маскируемых прерываний
#define EXTI_EMR    (*((volatile unsigned long*) 0x40010404)) // Установка маскируемых событий
#define EXTI_RTSR   (*((volatile unsigned long*) 0x40010408)) // Прерывание по фронту
#define EXTI_FRSR   (*((volatile unsigned long*) 0x4001040C)) // Прерывание по спаду
#define EXTI_SWIER  (*((volatile unsigned long*) 0x40010410)) // Программное прерывание
#define EXTI_PR     (*((volatile unsigned long*) 0x40010414)) // Установка задержки прерываний
  
// Регистры группы SPI, base adress 
#define SPI1_CR1			(*((volatile unsigned long *)0x40013000)) // Управляющий регистр 1 SPI1
#define SPI1_CR2			(*((volatile unsigned long *)0x40013004)) // Управляющий регистр 2 SPI1
#define SPI1_SR				(*((volatile unsigned long *)0x40013008)) // Регистр статуса SPI1
#define SPI1_DR				(*((volatile unsigned long *)0x4001300C)) // Регистр данных SPI1
  
// Регистры группы NVIC, base adress 0xE000E100
#define NVIC_ISER0  (*((volatile unsigned long*) 0xE000E100)) // Разрешение прерываний IRQ[31:0]
#define NVIC_ISER1  (*((volatile unsigned long*) 0xE000E104)) // Разрешение прерываний IRQ[63:32]
#define NVIC_ISER2  (*((volatile unsigned long*) 0xE000E108)) // Разрешение прерываний IRQ[80:64]
#define NVIC_ICER0	(*((volatile unsigned long*) 0xE000E180)) // Запрет прерываний IRQ[31:0]
#define NVIC_ICER1	(*((volatile unsigned long*) 0xE000E184)) // Запрет прерываний IRQ[63:32]
#define NVIC_ICER2	(*((volatile unsigned long*) 0xE000E188)) // Запрет прерываний IRQ[80:64]
#define NVIC_ISPR0  (*((volatile unsigned long*) 0xE000E200)) // Установка задержки прерываний IRQ[31:0]
#define NVIC_ISPR1  (*((volatile unsigned long*) 0xE000E204)) // Установка задержки прерываний IRQ[63:32]
#define NVIC_ISPR2  (*((volatile unsigned long*) 0xE000E208)) // Установка задержки прерываний IRQ[80:64]

