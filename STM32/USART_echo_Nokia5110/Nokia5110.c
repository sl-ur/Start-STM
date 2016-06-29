// Nokia5110.c
// Runs on LM4F120/TM4C123
// Use SSI0 to send an 8-bit code to the Nokia5110 48x84
// pixel LCD to display text, images, or other information.
// This file has been modified to work with TExaS, which
// sets the PLL to 80 MHz, where earlier versions ran at
// 50 MHz or less.
// Daniel Valvano
// December 29, 2014

// ���������� ��� STM32F100BR, STM32VL Discovery
// ������ ��������
// 29 ��� 2016 �.

// Font table, initialization, and other functions based
// off of Nokia_5110_Example from Spark Fun:
// 7-17-2011
// Spark Fun Electronics 2011
// Nathan Seidle
// http://dlnmh9ip6v2uc.cloudfront.net/datasheets/LCD/Monochrome/Nokia_5110_Example.pde

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA2
// SPI_NCC       (CE,  pin 2) connected to PA4
// Data/Command  (DC,  pin 3) connected to PA3
// SPI_TX        (Din, pin 4) connected to PA7
// SPI_SCK       (Clk, pin 5) connected to PA5
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SPI_NCC       (SCE, pin 3) connected to PA4
// Reset         (RST, pin 4) connected to PA2
// Data/Command  (D/C, pin 5) connected to PA3
// SPI_MOSI      (DN,  pin 6) connected to PA7
// SPO_SCK       (SCLK, pin 7) connected to PA5
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "Nokia5110.h"
// Maximum dimensions of the LCD, although the pixels are
// numbered from zero to (MAX-1).  Address may automatically
// be incremented after each transmission.
#define MAX_X     84
#define MAX_Y     48

#define DC_DATA_COMMAND       0x00000008  // ���� "1", ������� "0", ��3
#define RESET_HIGH_LOW        0x00000004  // ����� "0", ��2
#define RCC_APB2ENR		(*((volatile unsigned long *)0x40021018)) // ������� ������������ USART1, SPI1 � ������ �����/������
#define RCC_APB2ENR_SPI1EN		0x00001000	// ���������� ������������ SPI1
#define RCC_APB2ENR_IOPAEN		0x00000004	// ���������� ������������ ����� A
#define RCC_APB2ENR_AFIOEN		0x00000001	// ���������� ������������ �������������� ������� �����/������
#define SPI1_CR1			(*((volatile unsigned long *)0x40013000)) // ����������� ������� 1 SPI1
#define SPI1_CR2			(*((volatile unsigned long *)0x40013004)) // ����������� ������� 2 SPI1
#define SPI1_SR				(*((volatile unsigned long *)0x40013008)) // ������� ������� SPI1
#define SPI1_DR				(*((volatile unsigned long *)0x4001300C)) // ������� ������ SPI1
#define SPI1_CR1_DFF			    0x00000800 // ������ ������ ������, "0" - 8 ���, "1" - 16 ���
#define SPI1_CR1_LSBFIRST	    0x00000080 // ������� ��� ������ - "0", ������� ��� ������ - "1"
#define SPI1_CR1_SPE			    0x00000040 // ���������� SPI - "1"
// #define SPI1_CR1_BR				    0x00000018 // �������� ��������. 011 - 24/16 = 1,5 < 4
#define SPI1_CR1_BR				    0x00000010 // �������� ��������. 010 - 24/8 = 3,0 < 4
#define SPI1_CR1_MSTR			    0x00000004 // ������ - "1"
#define SPI1_CR1_CPOL			    0x00000002 // ���������� "1"
#define SPI1_CR1_CPHA			    0x00000001 //	���� "1"
#define SPI1_CR2_SSOE			    0x00000004 // ����� "����� ��������" "1"
#define SPI1_SR_BSY				    0x00000800 // ���� SPI ����� - "1"
#define SPI1_SR_TXE				    0x00000002 // ���� "������ ������" - "1"
#define GPIOA_CRL			(*((volatile unsigned long *)0x40010800)) // ���������� ������� ����� �, ��0 - ��7
#define GPIOA_CRH			(*((volatile unsigned long *)0x40010804)) // ���������� ������� ����� �, ��8 - ��15
#define GPIOA_ODR			(*((volatile unsigned long *)0x4001080C)) // ������� �������� ������ ����� �
#define GPIOA_BSRR    (*((volatile unsigned long *)0x40010810)) // ������� ���������/������ ��� ����� �
#define GPIOA_CRL_MODE_PA2	  0x00000100 // < 10 ���
#define GPIOA_CRL_CNF_PA2			// ����������� ����� (Reset)
#define GPIOA_CRL_MODE_PA3	  0x00001000 // < 10 ���
#define GPIOA_CRL_CNF_PA3			// ����������� ����� (Data)
#define GPIOA_CRL_MODE_PA4	  0x00010000 // < 10 ���
#define GPIOA_CRL_CNF_PA4		  0x00080000 // �������������� ����������� ����� (SPI1_NSS)
#define GPIOA_CRL_MODE_PA5	  0x00100000 // < 10 ���
#define GPIOA_CRL_CNF_PA5		  0x00800000 // �������������� ����������� ����� (SPI1_SCK)
#define GPIOA_CRL_MODE_PA7	  0x10000000 // < 10 ���
#define GPIOA_CRL_CNF_PA7		  0x80000000 // �������������� ����������� ����� (SPI1_MOSI)

// This table contains the hex values that represent pixels
// for a font that is 5 pixels wide and 8 pixels high
static const char ASCII[][5] = {
  {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c '\'
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
//  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
  ,{0x1f, 0x24, 0x7c, 0x24, 0x1f} // 7f UT sign
};

enum typeOfWrite{
  COMMAND,                              // the transmission is an LCD command
  DATA                                  // the transmission is data
};
// The Data/Command pin must be valid when the eighth bit is
// sent.  The SPI module has hardware input and output FIFOs
// that are 8 locations deep.  Based on the observation that
// the LCD interface tends to send a few commands and then a
// lot of data, the FIFOs are not used when writing
// commands, and they are used when writing data.  This
// ensures that the Data/Command pin status matches the byte
// that is actually being transmitted.
// The write command operation waits until all data has been
// sent, configures the Data/Command pin for commands, sends
// the command, and then waits for the transmission to
// finish.
// The write data operation waits until there is room in the
// transmit FIFO, configures the Data/Command pin for data,
// and then adds the data to the transmit FIFO.

// This is a helper function that sends an 8-bit message to the LCD.
// inputs: type     COMMAND or DATA
//         message  8-bit code to transmit
// outputs: none
// assumes: SPI1 and port A have already been initialized and enabled
void static lcdwrite(enum typeOfWrite type, char message){
    unsigned long delay;
    if(type == COMMAND){
                                   // wait until SPI1 not busy/transmit FIFO empty
    while((SPI1_SR & SPI1_SR_BSY) == SPI1_SR_BSY){};
    GPIOA_ODR &= ~DC_DATA_COMMAND; // �������� �������
    for(delay=0; delay<10; delay=delay+1); // �������� ����� ������ ������� � ������ ���������� ��� 100 ��
    SPI1_DR = message;             // command out
                                   // wait until SPI1 not busy/transmit FIFO empty
    while((SPI1_SR & SPI1_SR_BSY) == SPI1_SR_BSY){};
  } else{
    while(((SPI1_SR & SPI1_SR_TXE) >> 1) == 0){}; // wait until transmit FIFO not full
    GPIOA_ODR |= DC_DATA_COMMAND;  // �������� ����������
    for(delay=0; delay<10; delay=delay+1); // �������� ����� ������ ������� � ������ ���������� ��� 100 ��
    SPI1_DR = message;             // data out
  }
}

//********Nokia5110_Init*****************
// Initialize Nokia 5110 48x84 LCD by sending the proper
// commands to the PCD8544 driver.  One new feature of the
// LM4F120 is that its SSIs can get their baud clock from
// either the system clock or from the 16 MHz precision
// internal oscillator.
// inputs: none
// outputs: none
// assumes: system clock rate of 80 MHz
void Nokia5110_Init(void){
// ������������� �������
  volatile unsigned long delay;
	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN; // ����������� SPI
	RCC_APB2ENR	|= RCC_APB2ENR_IOPAEN; // ����������� ����� �
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN; // ����������� �������������� �������
	delay = RCC_APB2ENR; // 
	GPIOA_CRL &= ~0xF0FFFF00;          // ��������� ����� ��������� ������ PA 2-5, 7
	GPIOA_CRL |= (GPIOA_CRL_MODE_PA2 | // ����� < 10 ���
/*							GPIOA_CRL_CNF_PA2	 | // �������� ����������� �����, 00 (push-pull) */
								GPIOA_CRL_MODE_PA3); // ����� < 10 ���
/*							GPIOA_CRL_CNF_PA3);	 // �������� ����������� �����, 00 (push-pull) */
	GPIOA_CRL |= (GPIOA_CRL_MODE_PA4 | // ����� < 10 ���
								GPIOA_CRL_CNF_PA4	 | // �������������� ����������� ����� (push-pull) (SPI)
								GPIOA_CRL_MODE_PA5 | // ����� < 10 ���
								GPIOA_CRL_CNF_PA5  | // �������������� ����������� ����� (push-pull) (SPI)
								GPIOA_CRL_MODE_PA7 | // ����� < 10 ���
								GPIOA_CRL_CNF_PA7);  // �������������� ����������� ����� (push-pull) (SPI)
	SPI1_CR1 &= ~SPI1_CR1_SPE;				 // ������ SPI1
	SPI1_CR1 &= (~SPI1_CR1_DFF &			 // ��������� ������ ������ 8 ���
							~SPI1_CR1_LSBFIRST &	 // ��������� �������� �������� ���� ������				 
							~SPI1_CR1_CPOL & 			 // ��������� ���������� 0
							~SPI1_CR1_CPHA); 			 // ��������� ���� 0
	SPI1_CR1 |= (SPI1_CR1_BR | 				 // ��������� �������� �������� �������� /16
							 SPI1_CR1_MSTR); 		 // ��������� ������ SPI						
	SPI1_CR2 |= SPI1_CR2_SSOE; 				 // ����������� ���������� ������ NSS
	SPI1_CR1 |= SPI1_CR1_SPE;					 // ���������� SPI1
// ������������� �������
  GPIOA_ODR &= ~RESET_HIGH_LOW;         // reset the LCD to a known state
  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
  GPIOA_ODR |= RESET_HIGH_LOW;          // negative logic
  lcdwrite(COMMAND, 0x21);              // chip active; horizontal addressing mode (V = 0); use extended instruction set (H = 1)
                                        // set LCD Vop (contrast), which may require some tweaking:
  lcdwrite(COMMAND, CONTRAST);          // try 0xB1 (for 3.3V red SparkFun), 0xB8 (for 3.3V blue SparkFun), 0xBF if your display is too dark, or 0x80 to 0xFF if experimenting
  lcdwrite(COMMAND, 0x04);              // set temp coefficient
  lcdwrite(COMMAND, 0x14);              // LCD bias mode 1:48: try 0x13 or 0x14
  lcdwrite(COMMAND, 0x20);              // we must send 0x20 before modifying the display control mode
  lcdwrite(COMMAND, 0x0C);              // set display control to normal mode: 0x0D for inverse
}

//********Nokia5110_OutChar*****************
// Print a character to the Nokia 5110 48x84 LCD.  The
// character will be printed at the current cursor position,
// the cursor will automatically be updated, and it will
// wrap to the next row or back to the top if necessary.
// One blank column of pixels will be printed on either side
// of the character for readability.  Since characters are 8
// pixels tall and 5 pixels wide, 12 characters fit per row,
// and there are six rows.
// inputs: data  character to print
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutChar(unsigned char data){
  int i;
  lcdwrite(DATA, 0x00);                 // blank vertical line padding
  for(i=0; i<5; i=i+1){
    lcdwrite(DATA, ASCII[data - 0x20][i]);
  }
  lcdwrite(DATA, 0x00);                 // blank vertical line padding
}

//********Nokia5110_OutString*****************
// Print a string of characters to the Nokia 5110 48x84 LCD.
// The string will automatically wrap, so padding spaces may
// be needed to make the output look optimal.
// inputs: ptr  pointer to NULL-terminated ASCII string
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutString(unsigned char *ptr){
  while(*ptr){
    Nokia5110_OutChar((unsigned char)*ptr);
    ptr = ptr + 1;
  }
}

//********Nokia5110_OutUDec*****************
// Output a 16-bit number in unsigned decimal format with a
// fixed size of five right-justified digits of output.
// Inputs: n  16-bit unsigned number
// Outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_OutUDec(unsigned short n){
  if(n < 10){
    Nokia5110_OutString("    ");
    Nokia5110_OutChar(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    Nokia5110_OutString("   ");
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  } else if(n<1000){
    Nokia5110_OutString("  ");
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    Nokia5110_OutChar(' ');
    Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
  else {
    Nokia5110_OutChar(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
}

//********Nokia5110_SetCursor*****************
// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=11)
//         newY  new Y-position of the cursor (0<=newY<=5)
// outputs: none
void Nokia5110_SetCursor(unsigned char newX, unsigned char newY){
  if((newX > 11) || (newY > 5)){        // bad input
    return;                             // do nothing
  }
  // multiply newX by 7 because each character is 7 columns wide
  lcdwrite(COMMAND, 0x80|(newX*7));     // setting bit 7 updates X-position
  lcdwrite(COMMAND, 0x40|newY);         // setting bit 6 updates Y-position
}

//********Nokia5110_Clear*****************
// Clear the LCD by writing zeros to the entire screen and
// reset the cursor to (0,0) (top left corner of screen).
// inputs: none
// outputs: none
void Nokia5110_Clear(void){
  int i;
  for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){
    lcdwrite(DATA, 0x00);
  }
  Nokia5110_SetCursor(0, 0);
}

//********Nokia5110_DrawFullImage*****************
// Fill the whole screen by drawing a 48x84 bitmap image.
// inputs: ptr  pointer to 504 byte bitmap
// outputs: none
// assumes: LCD is in default horizontal addressing mode (V = 0)
void Nokia5110_DrawFullImage(const char *ptr){
  int i;
  Nokia5110_SetCursor(0, 0);
  for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){
    lcdwrite(DATA, ptr[i]);
  }
}
