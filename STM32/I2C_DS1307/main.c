/**
* main.c
* I2C1 для DS1307 (часы реального времени RTC)
* Вывод времени на дисплей Nokia5110
* Использование имен регистров и битов, определенные через define.
* Использование PLL, 24 МГц.
* Кварц 8 МГц.
* Настройка только через регистры.
* Keil 5.20
* STM32F100RB
* DS1307 (Модуль Tiny RTS I2C)
* Nokia5110
* V.0.01 Только чтение текущего времени из RTC DS1307 и первичная установка
* 09.06.2016
* 27.06.2016 программа заработала
* Установка начального времени и даты по нажатию кнопки (прерывание)
* Сделать выход 1 Гц в DS1307 и по внешнему прерываню запускать считывания часов
*/

#include "def_f100x.h"
#include "PLL.h"
#include "SysTick.h"
#include "I2C.h"
#include "SPI.h"
#include "ds1307.h"
#include "Nokia5110.h"
	
 void PortA_Init(void);
 void PortC_Init(void);

unsigned long SetTime;
void EXTI0_IRQHandler(void){
	SysTick_Wait10ms(20); // задержка 200 мс от дребезга контактов кнопки
  EXTI_PR |= 0x00000001; // 
  SetTime = 1;
}
 


// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power 3.3V
// Ground        (GND, pin 2) ground
// SPI_NCC       (SCE, pin 3) connected to PA4
// Reset         (RST, pin 4) connected to PA2
// Data/Command  (D/C, pin 5) connected to PA3
// SPI_MOSI      (DN,  pin 6) connected to PA7
// SPO_SCK       (SCLK, pin 7) connected to PA5
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

// Модуль Tiny RTS I2C
// -------------------
// Signal        (Tiny RTS I2C) LaunchPad pin
// 5.0V          (VCC, pin 1) 	power 5.0V
// Ground        (GND, pin 2) 	ground
// SCL           (SCL, pin  )   PB6
// SDA           (SDA, pin  )   PB7

int main()
{	
	unsigned char clock[64] = {0}; // Считанные данные регистров часов
	// Установка 0-го регистра, времени и даты. 16:00:00, воскресение и 19.06.16, 24 часа
	// В дальнешем предусматреть установку управляющего регистра, set[9]
	unsigned char set[8] = {0x0, 0x0, 0x22, 0x22, 0x01, 0x27, 0x06, 0x16};
  char data;
	
	PLL_Init();
  SPI_Init();
	SysTick_Init();
  DS1307_Init();
  Nokia5110_Init();
	PortC_Init();
	PortA_Init();
	I2C1_Init();
	
// Зеленый светодиод выкл, PC9. Синий светодиод вкл, PC8. Положительная логика
	GPIOC_BSRR = 0x2000100;
  Nokia5110_Clear();
  Nokia5110_OutString("DS1307");
	Nokia5110_SetCursor(0, 1);
	Nokia5110_OutString("------");
	
//	I2C1_Transmit(set, 8, ADDR_DS1307);
	
	while(1){
// Установка времени и даты, если нажата кнопка
		if(SetTime == 1){
			I2C1_Transmit(set, 8, ADDR_DS1307);
			SetTime = 0;
		}
  // Чтение регистров часов и календаря с адреса 0
		clock[0] = 0;
		I2C1_Transmit(clock, 1, ADDR_DS1307);
		I2C1_Receive(clock, 7, ADDR_DS1307);
		// Вывод на дисплей Nokia5110
		// Проверка бита CH
		Nokia5110_SetCursor(0, 5);
		Nokia5110_OutString("CH: ");
		data = (clock[0] >> 7) & 0x1;
		Nokia5110_OutBCD(data);
	// Время
		Nokia5110_SetCursor(0, 2);
		// часы
		data = (clock[2] >> 4) & 0x3; // 10-ки часов
		Nokia5110_OutUDec(data);
		data = clock[2] & 0xF; // Единицы часов
		Nokia5110_OutBCD(data);
		Nokia5110_OutChar(':');
		// минуты
		data = (clock[1] >> 4) & 0x7; // 10-ки минут
		Nokia5110_OutBCD(data);
		data = clock[1] & 0xF; // Единицы минут
		Nokia5110_OutBCD(data);
		Nokia5110_OutChar(':');
		// секунды
		data = (clock[0] >> 4) & 0x7; // 10-ки секунд
		Nokia5110_OutBCD(data);
		data = clock[0] & 0xF; // Единицы секунд
		Nokia5110_OutBCD(data);
	// Календарь
		// День
		Nokia5110_SetCursor(0, 3);
		data = (clock[4] >> 4) & 0x3; // 10-ки дней
		Nokia5110_OutBCD(data);
		data = clock[4] & 0xF; // Единицы дней
		Nokia5110_OutBCD(data);
		Nokia5110_OutChar('.');
		// Месяц
		data = (clock[5] >> 4) & 0x1; // 10-ки месяца
		Nokia5110_OutBCD(data);
		data = clock[5] & 0xF; // Единицы месяца
		Nokia5110_OutBCD(data);
		Nokia5110_OutChar('.');
		// Год
		data = (clock[6] >> 4) & 0xF; // 10-ки года
		Nokia5110_OutBCD(data);
		data = clock[6] & 0xF; // Единицы года
		Nokia5110_OutBCD(data);
		SysTick_Wait10ms(50);
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
 // GPIOC_CRL &=(~0xF0000000);
 // GPIOC_CRL |= 0x20000000;
}

