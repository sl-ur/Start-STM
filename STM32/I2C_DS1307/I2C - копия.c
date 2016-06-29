// I2C.c
// ������ ��������
// 09 ���� 2016

// � ������� ������� �������� �� ������������ �������� �������,
// ��� �� �� ������ � ����������� �������� � ������ �����-���� ������.

// ������ Tiny RTS I2C
// PB6 - I2C1_SCL 
// PB7 - I2C1_SDA

#include "I2C.h"

volatile unsigned long Read_SR1;
volatile unsigned long Read_SR2;

// ������������� I2C1. PB6 - I2C1_SCL. PB7 - I2C1_SDA.
void I2C1_Init(void){
	// ���������� ������������
	RCC_APB2ENR |= RCC_APB2ENR_IOPBEN; // ��������� PortB
	RCC_APB1ENR |= RCC_APB1ENR_I2C1EN; // ��������� I2C1
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN; // ����������� �������������� �������
	// ��������� ������� I2C1 SDA - PB7 � SCL - PB6 � �������������� ������
	GPIOB_CRL &= ~0xFF000000; // ����� ����� ��������� ����� PB7 � PB6
	GPIOB_CRL |= (GPIOB_CRL_MODE_50MHZ_PB6 |
								GPIOB_CRL_CNF_OUTOD_PB6	 |
								GPIOB_CRL_MODE_50MHZ_PB7 |
								GPIOB_CRL_CNF_OUTOD_PB7);
	// ����� I2C1
	RCC_APB1RSTR |= RCC_APB1RSTR_I2C1RST;  // ��������� �������� ��������� 
	RCC_APB1RSTR &= ~RCC_APB1RSTR_I2C1RST; // ���� ��������� I2C1
	// ���������������� I2C1
	I2C1_CR2 &= ~0x3F;
	I2C1_CR2 |= I2C1_CR2_FREQ_24MHZ; // ������� ������������ ������ I2C1
	I2C1_CCR &= ~I2C1_CCR_FS_FAST;   // ����� Standart, 100 ���
	I2C1_CCR &= ~I2C1_CCR_DUTY;	   	 // ���� �������� ��� ������ Fast
//	I2C1_CCR &= ~0xFFF;
	I2C1_CCR |= I2C1_CCR_CCR;		 	 	 // 
	I2C1_TRISE |= I2C1_TRISE_TRISE;  // 
	I2C1_CR1 &= ~I2C1_CR1_SMBUS;		 // ��������� ������ "I2C"
	I2C1_CR1 |= I2C1_CR1_PE; 				 // ���������� ������������ ���������
	I2C1_CR1 |= I2C1_CR1_ACK;				 //
}

// �������� ���������� ������������ I2C
int I2C_GetFlagSatusBUSY(void){
  while((I2C1_SR2 & I2C1_SR2_BUSY) == I2C1_SR2_BUSY){};
  return SUCCESS;
}

// ������������� ������� Start
void I2C_Init_Start(void){
  I2C1_CR1 |= I2C1_CR1_START;
}

// �������� ������� ������� Start, EV5
int I2C_CheckEventSBbit(void){
  while((I2C1_SR1 & I2C1_SR1_SB) == I2C1_SR1_SB){};
//  I2C1_SR1;
	return SUCCESS;
}

// �������� ������ ��������
void I2C_Send7bitAddress(unsigned char SlaveAddress, unsigned char I2C_DirectionTransmitter){
  SlaveAddress = (SlaveAddress << 1) | I2C_DirectionTransmitter;
  I2C1_DR = SlaveAddress;
}

// �������� ������� ������� �������� �� ����������� ������, EV6
// 1) ������ SR1 (��� 573 RM0041)
// 2) �������� ��������� ADDR
// 3) ������ SR2
int I2C_CheckEventADDR(void){
  while((I2C1_SR1 & I2C1_SR1_ADDR) == I2C1_SR1_ADDR){};
  Read_SR2 = I2C1_SR2;
  return SUCCESS;
}

// �������� "������ ������" TxE, EV8
int I2C_CheckEventTXE(void){
  while((I2C1_SR1 & I2C1_SR1_TXE) == I2C1_SR1_TXE);
  return SUCCESS;
}

// �������� �����
void I2C_SendByte(const unsigned char* buffer){
  I2C1_DR = *buffer; // ����� ���� �� �������!!!!!!!!!!!!!!!
}

// �������� ���������� �������� �����, ���� BTF
int I2C_CheckBTF(void){
  while((I2C1_SR1 & I2C1_SR1_BTF) == I2C1_SR1_BTF);
  return SUCCESS;
}

// �������� ������� ��� ������ ������� Stop, EV8_2 (TXE = 1 � BTF = 1), EV8_2
int I2C_Check_TXEBTF(void){
	while(((I2C1_SR1 & I2C1_SR1_TXE) == I2C1_SR1_TXE) &&
			  ((I2C1_SR1 & I2C1_SR1_TXE) == I2C1_SR1_TXE));
	return SUCCESS;
}

// ������������� ������� Stop
void I2C_InitStop(void){
  I2C1_CR1 |= I2C1_CR1_STOP;
}

// �������� ������� ������� Stop, TxE = 0, BTF = 0
int I2C_CheckEventStop(void){
 // while(!I2C_CheckEventTXE & !I2C_CheckEventTXE){};
	return SUCCESS;
}

// ���������� ������������� - ACK
void I2C_AcknowledgeConfig(int PERMIT){
  if(PERMIT)
    I2C1_CR1 |= I2C1_CR1_ACK;
  else
    I2C1_CR1 &= ~I2C1_CR1_ACK;
}

// ���������� ������ POS
void I2C_NACK_PositionConfig(int POSITION){
  if(NextByte)
    I2C1_CR1 |= I2C1_CR1_POS;
  else
    I2C1_CR1 &= ~I2C1_CR1_POS;
}

// ����� �����
unsigned char I2C_ReceivByte(void){
  unsigned char rbyte;
  rbyte = I2C1_DR;
  return rbyte;
}

// ��������� ����� RXNE
int I2C_GetFlagStatusRXNE(void){
  while((I2C1_SR1 & I2C1_SR1_RXNE) == I2C1_SR1_RXNE){};
  return SUCCESS;
}

// �������� I2C1
int I2C1_Transmit(const unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress){
	unsigned char I2C1_DirectionTransmitter = 0; // ��������
	if(nbyte > 0){
    I2C_GetFlagSatusBUSY();
// ������������� ������� Start
    I2C_Init_Start();
// �������� ������� ������� Start, EV5
    I2C_CheckEventSBbit();
// �������� ������ ��������
    I2C_Send7bitAddress(SlaveAddress, I2C1_DirectionTransmitter);
// �������� ������� ������� �������� �� ����������� ������, EV6
// 1) ������ SR1 (��� 573 RM0041)
// 2) �������� ��������� ADDR
// 3) ������ SR2
    I2C_CheckEventADDR();
// �������� "������ ������" TxE
//    I2C_CheckEventTXE();
// �������� ������� �����
    I2C_SendByte(buffer++);
		while(--nbyte > 0){
// �������� ���������� �������� �����, ���� BTF
			I2C_CheckBTF();
//			I2C_CheckEventTXE();
			I2C_SendByte(buffer++);
		}
// �������� ������� ��� ������ ������� Stop, EV8_2 (TXE = 1 � BTF = 1), EV8_2
		I2C_Check_TXEBTF();
// ������������� ������� Stop
//    I2C_CheckBTF();
		I2C_InitStop();
// �������� ������� ������� Stop, TxE = 0, BTF = 0
//    I2C_CheckEventStop();
  }
	
	return SUCCESS;
}

// ����� I2C1
int I2C1_Receive(unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress){
  unsigned char I2C1_DirectionTransmitter = 1; // �����
  if(nbyte == 0)
    return SUCCESS;
// �������� ����������� I2C
//  I2C_GetFlagSatusBUSY();
// �������� ������������� � ����� POC
  I2C_AcknowledgeConfig(ENABLE);
  I2C_NACK_PositionConfig(CurrentByte);
// ������������� ������� Start
  I2C_Init_Start();
// �������� ������ ��������, EV5
  I2C_Send7bitAddress(SlaveAddress, I2C1_DirectionTransmitter);
// EV6
  I2C_CheckEventADDR();
  if(nbyte == 1){ // ������ 1-�� �����
    // ����� ���� ACK
    I2C_AcknowledgeConfig(DISABLE);
    // EV6_1
    Read_SR1 = I2C1_SR1;
		Read_SR2 = I2C1_SR2; // ����� ���� ADDR  
    I2C_InitStop(); // ������������� ������� Stop
    // ����� ����� EV7
    I2C_GetFlagStatusRXNE();
    *buffer++ = I2C_ReceivByte();
  }
  else if(nbyte == 2){ // ������ 2-� ����
    // ��������� ����� POS
    I2C_NACK_PositionConfig(NextByte);
    // EV6_1
    Read_SR1 = I2C1_SR1;
		Read_SR2 = I2C1_SR2; // ����� ���� ADDR
    I2C_AcknowledgeConfig(DISABLE); // ����� ���� ACK
    // EV7_3, �������� BTF, ��������� ��������� � ����� ����� ������
//    I2C_CheckBTF();
    I2C_InitStop(); // ������������� ������� Stop
    *buffer++ = I2C_ReceivByte();
    *buffer++ = I2C_ReceivByte();
  }
  else{ // ������ 3-� � ����� ������
    Read_SR1 = I2C1_SR1;
		Read_SR2 = I2C1_SR2; // ����� ���� ADDR
    while(nbyte-- != 3){
      // EV7 - ��� ��������, ������� ������ RXNE �������� BTF
      // �������� BTF
      I2C_CheckBTF();
      *buffer++ = I2C_ReceivByte();
    }
    // EV7_2
    I2C_CheckBTF();
		I2C_AcknowledgeConfig(DISABLE); // ����� ���� ACK
    *buffer++ = I2C_ReceivByte(); // ������ N-2 �����
    I2C_InitStop(); // ������������� ������� Stop
    *buffer++ = I2C_ReceivByte(); // ������ N-1 �����
    // �������� ����� N
    I2C_GetFlagStatusRXNE();
    *buffer++ = I2C_ReceivByte(); // ������ N �����
    
    nbyte = 0;
  }
  return SUCCESS;
}
