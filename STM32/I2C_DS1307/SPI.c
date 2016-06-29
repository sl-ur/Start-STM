// SPI.c
// ������ ��������
// 09 ���� 2016

#include "SPI.h"

  // ������������� ������� SPI1
void SPI_Init(void){
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
							 SPI1_CR1_MSTR); 		   // ��������� ������ SPI						
	SPI1_CR2 |= SPI1_CR2_SSOE; 				 // ����������� ���������� ������ NSS
	SPI1_CR1 |= SPI1_CR1_SPE;					 // ���������� SPI1
}
