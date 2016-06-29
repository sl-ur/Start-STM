// I2C.h
// ������ ��������
// 09 ���� 2016
// ������ ������ ������ ��� �������.
// � ������� ������� �������� �� ������������ �������� �������,
// ��� �� �� ������ � ����������� �������� � ������ �����-���� ������.
// � ����������� �� �����������.
// ������� ����� 1 �� � DS1307 � �� �������� ��������� ��������� ���������� �����

#include "def_f100x.h"

#define GPIOB_CRL_MODE_50MHZ_PB6	  0x03000000 // < 50 ���
#define GPIOB_CRL_CNF_OUTOD_PB6			0x0C000000 // ����� � �������� ����������� (od)
#define GPIOB_CRL_MODE_50MHZ_PB7	  0x30000000 // < 50 ���
#define GPIOB_CRL_CNF_OUTOD_PB7			0xC0000000 // ����� � �������� ����������� (od)
#define I2C1_CR1_ACK                0x400
#define I2C1_CR2_FREQ_24MHZ					0x18	 // 0b011000 ������� ������������ ������ I2C1
#define I2C1_CCR_FS_FAST						0x8000 // Fast Mode - '1', Standart Mode - '0'
#define I2C1_CCR_DUTY								0x4000 // 2 - '0', 16/9 - '1'
#define I2C1_CCR_CCR								113 	 // (����������� ����� 100 ���, ������������ ������ 24 ���
#define I2C1_TRISE_TRISE						25  	 // ������������ ��������� ������ � ������ Master
// ������
#define I2C1_CR1_SMBUS							0x2 	 // '0' - I2C, '1' - SMBUS
#define I2C1_CR1_PE									0x1 	 // ���������� ��������� I2C
#define I2C1_CR1_START							0x100
#define I2C1_CR1_STOP               0x200
#define I2C1_SR1_SB                 0x1
#define I2C1_SR1_ADDR               0x2
#define I2C1_SR1_BTF                0x4
#define I2C1_SR1_TXE                0x80
#define I2C1_SR1_RXNE               0x40
#define I2C1_SR2_TRA                0x4 // '0' - ��������, '1' - ����������
#define I2C1_SR2_BUSY               0x2 // '1' - ���� �����, '0' - ����� ��������
#define I2C1_SR2_MSL								0x1 // '0' - �������, '1' - �������
// ������
#define I2C1_CR1_ACK                0x400
#define I2C1_CR1_POS                0x800
#define I2C1_SR1_STOPF              0x10

#define SUCCESS											0
#define FAULT												1 
#define ENABLE                      1 // ���������� ������������ ACK
#define DISABLE                     0 // ������ ������������� ACK
#define CurrentByte                 0 // POS
#define NextByte                    1 // POS

// ������������� I2C1.
void I2C1_Init(void);
// �������� ���������� ������������ (�����������) I2C
int I2C_GetFlagSatusBUSY(void);
// ������������� ������� Start
void I2C_Init_Start(void);
// �������� ������� ������� Start
int I2C_CheckEventSBbit(void);
// �������� ������ ��������, EV5
void I2C_Send7bitAddress(unsigned char SlaveAddress, unsigned char I2C1_DirectionTransmitter);
// �������� ������� ������� �������� �� ����������� ������
// 1) ������ SR1 (��� 573 RM0041)
// 2) �������� ��������� ADDR
// 3) ������ SR2
int I2C_CheckEventADDR(void);
// �������� "������ ������" TxE
int I2C_CheckEventTXE(void);
// �������� �����
void I2C_SendByte(const unsigned char* buffer);
// �������� ���������� �������� �����, ���� BTF
int I2C_CheckBTF(void);
// �������� ������� ��� ������ ������� Stop, EV8_2 (TXE = 1 � BTF = 1)
int I2C_Check_TXEBTF(void);
// ������������� ������� Stop
void I2C_InitStop(void);
// �������� ������� ������� Stop, TxE = 0, BTF = 0
int I2C_CheckEventStop(void);
// ���������� �������������
void I2C_AcknowledgeConfig(int PERMIT);
// ���������� ������ POS
void I2C_NACK_PositionConfig(int POSITION);
// ����� �����
unsigned char I2C_ReceivByte(void);
// ��������� ����� RXNE
int I2C_GetFlagStatusRXNE(void);

// �������� I2C1
int I2C1_Transmit(const unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress);

// ����� I2C1
int I2C1_Receive(unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress);
