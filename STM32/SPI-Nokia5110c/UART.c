// UART.c
// Runs on LM3S811, LM3S1968, LM3S8962, LM4F120, TM4C123
// Simple device driver for the UART.
// Daniel Valvano
// September 11, 2013
// Modified by EE345L students Charlie Gough && Matt Hawk
// Modified by EE345M students Agustinus Darmawan && Mingjie Qiu
// Изменен Карповым Вячеславом 23.05.2016 для STM32 VL Discovery 32STM32F100RB

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Program 4.12, Section 4.9.4, Figures 4.26 and 4.40

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U1Rx (VCP receive) connected to PA10
// U1Tx (VCP transmit) connected to PA9

#include "UART.h"

#define USART_SR      (*((volatile unsigned long *)0x40013800)) // Регистр статуса USART
#define USART_DR      (*((volatile unsigned long *)0x40013804)) // Регистр данных USART
#define USART_BRR     (*((volatile unsigned long *)0x40013808)) // Регистр установки скорости передачи 
#define USART_CR1     (*((volatile unsigned long *)0x4001380C)) // Регистр 1 управления 
#define USART_CR2     (*((volatile unsigned long *)0x40013810)) // Регистр 2 управления
#define USART_CR3     (*((volatile unsigned long *)0x40013814)) // Регистр 3 управления
#define USART_GTPR    (*((volatile unsigned long *)0x40013818)) // Регистр ...
#define USART_CR1_UE			0x00002000	// Разрешение USART
#define USART_CR1_M			0x00001000	// Длина слова. M = 1 - 9 бит, M = 0 - 8 бит
// #define USART_CR2_STOP		0x00001000	// Количество битов STOP - 1 бит
// Скорость передачи 115200. Fck = 24 МГц
#define USART_BRR_DIV_Mantissa	0x000000D0	// 
#define USART_BRR_DIV_Fraction	0x00000000	//
#define USART_CR1_TE			0x00000008	// Бит разрешения передачи. "1" передача разрешена
#define USART_SR_TXE			0x00000080	// Флаг, регистр передачи пустой
#define USART_SR_TC				0x00000040	// Флаг, передача закончена
#define USART_CR1_RE			0x00000004	// Бит разрешения приема. "1" прием разрешен
#define USART_SR_RXNE			0x00000020	// флаг, регистр приема не пустой
#define RCC_APB2ENR		(*((volatile unsigned long *)0x40021018)) // Регистр тактирования USART1 и портов ввода/вывода
#define RCC_APB2ENR_USART1EN	0x00004000	// Разрешение тактирование USART1
#define RCC_APB2ENR_IOPAEN		0x00000004	// Разрешение тактирования порта 
#define RCC_ARB2ENR_AFIOEN		0x00000001	// Разрешение тактирования альтернативных функция входа/выхода
#define GPIOA_CRH			(*((volatile unsigned long *)0x40010804)) // Установка порта А 
#define GPIOA_CRH_CNF_PA9		0x08000000	// Установка альтернативной функции (USART1TX) порта A9 на выход, двухтактный выход (push-poll)
#define GPIOA_CRH_MODE_PA9	0x01000000	// режим <=10 МГц
#define GPIOA_CRH_CNF_PA10	0x00400000	// Установка альтернативной функции (USART1RX) порта A10 на вход, плавующий вход (input floating)
#define GPIOA_CRH_MODE_PA10	0x00000000	// Если вход, то MODE всегда "0"

//------------UART_Init------------
// Initialize the UART for 115,200 baud rate (assuming 24 MHz UART clock),
// 8 bit word length, no parity bits, one stop bit
// Input: none
// Output: none
void UART_Init(void){
	RCC_APB2ENR |= RCC_APB2ENR_USART1EN;	// 1) Активация USART1
	RCC_APB2ENR |= RCC_APB2ENR_IOPAEN; 		// 2) Активация порта A
	RCC_APB2ENR	|= RCC_ARB2ENR_AFIOEN;		// 3) Активация альтернативной функции входа/выхода
	USART_CR1 &= ~USART_CR1_UE; 					// 4) Запрет работы USART
	USART_CR1 &= ~USART_CR1_M; 						// 5)Установка длины слова - 8 бит
	USART_CR2 &= ~0x00003000; 						// 6) Установка количества битов STOP - 1 бит
//	USART_BRR_DIV_Mantissa	0x0000xxx0	//
//	USART_BRR_DIV_Fraction	0x0000000x	//
	USART_BRR = USART_BRR_DIV_Mantissa + USART_BRR_DIV_Fraction; // 7)
	USART_CR1 |= USART_CR1_TE; 						// 8) Разарешение передачи
	USART_CR1 |= USART_CR1_RE; 						// 9) Разрешение приема
	USART_CR1 |= USART_CR1_UE; 						// 10) Разрешение работы USART
	GPIOA_CRH &= ~0x0FF00000;							// 11) Очистка битов режима портов PA9 И PA10
	GPIOA_CRH |= GPIOA_CRH_MODE_PA9 | GPIOA_CRH_CNF_PA9; // Установка режима порта PA9
	GPIOA_CRH	|= GPIOA_CRH_MODE_PA10 | GPIOA_CRH_CNF_PA10; // Установка режима порта PA10
}

//------------UART_InChar------------
// Wait for new serial port input
// Input: none
// Output: ASCII code for key typed
unsigned char UART_InChar(void){
	while(((USART_SR & USART_SR_RXNE) >> 5) != 1) {}; // Информация принята и может быть считана
	return((unsigned char) (USART_DR & 0xFF));
}
//------------UART_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(unsigned char data){
  while(((USART_SR & USART_SR_TXE) >> 7) != 1) {};
	USART_SR &= ~USART_SR_TC; // 
  USART_DR = data & 0xFF;
}


//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt){
  while(*pt){
    UART_OutChar(*pt);
    pt++;
  }
}

//------------UART_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long UART_InUDec(void){
unsigned long number=0, length=0;
char character;
  character = UART_InChar();
  while(character != CR){ // accepts until <enter> is typed
// The next line checks that the input is a digit, 0-9.
// If the character is not 0-9, it is ignored and not echoed
    if((character>='0') && (character<='9')) {
      number = 10*number+(character-'0');   // this line overflows if above 4294967295
      length++;
      UART_OutChar(character);
    }
// If the input is a backspace, then the return number is
// changed and a backspace is outputted to the screen
    else if((character==BS) && length){
      number /= 10;
      length--;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  return number;
}

//-----------------------UART_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_OutUDec(unsigned long n){
// This function uses recursion to convert decimal number
//   of unspecified length as an ASCII string
  if(n >= 10){
    UART_OutUDec(n/10);
    n = n%10;
  }
  UART_OutChar(n+'0'); /* n is between 0 and 9 */
}

//---------------------UART_InUHex----------------------------------------
// Accepts ASCII input in unsigned hexadecimal (base 16) format
// Input: none
// Output: 32-bit unsigned number
// No '$' or '0x' need be entered, just the 1 to 8 hex digits
// It will convert lower case a-f to uppercase A-F
//     and converts to a 16 bit unsigned number
//     value range is 0 to FFFFFFFF
// If you enter a number above FFFFFFFF, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long UART_InUHex(void){
unsigned long number=0, digit, length=0;
char character;
  character = UART_InChar();
  while(character != CR){
    digit = 0x10; // assume bad
    if((character>='0') && (character<='9')){
      digit = character-'0';
    }
    else if((character>='A') && (character<='F')){
      digit = (character-'A')+0xA;
    }
    else if((character>='a') && (character<='f')){
      digit = (character-'a')+0xA;
    }
// If the character is not 0-9 or A-F, it is ignored and not echoed
    if(digit <= 0xF){
      number = number*0x10+digit;
      length++;
      UART_OutChar(character);
    }
// Backspace outputted and return value changed if a backspace is inputted
    else if((character==BS) && length){
      number /= 0x10;
      length--;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  return number;
}

//--------------------------UART_OutUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void UART_OutUHex(unsigned long number){
// This function uses recursion to convert the number of
//   unspecified length as an ASCII string
  if(number >= 0x10){
    UART_OutUHex(number/0x10);
    UART_OutUHex(number%0x10);
  }
  else{
    if(number < 0xA){
      UART_OutChar(number+'0');
     }
    else{
      UART_OutChar((number-0x0A)+'A');
    }
  }
}

//------------UART_InString------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void UART_InString(char *bufPt, unsigned short max) {
int length=0;
char character;
  character = UART_InChar();
  while(character != CR){
    if(character == BS){
      if(length){
        bufPt--;
        length--;
        UART_OutChar(BS);
      }
    }
    else if(length < max){
      *bufPt = character;
      bufPt++;
      length++;
      UART_OutChar(character);
    }
    character = UART_InChar();
  }
  *bufPt = 0;
}
