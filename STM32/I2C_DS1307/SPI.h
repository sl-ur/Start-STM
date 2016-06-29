// SPI.h
// ������ ��������
// 09 ���� 2016

#include "def_f100x.h"

// #define RCC_APB2ENR		(*((volatile unsigned long *)0x40021018)) // ������� ������������ USART1, SPI1 � ������ �����/������
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

void SPI_Init(void);
