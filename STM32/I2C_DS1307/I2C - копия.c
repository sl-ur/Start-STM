// I2C.c
// Карпов Вячеслав
// 09 июня 2016

// В будущем сделать проверку на длительность ожидания статуса,
// что бы не впасть в бесконечное ожидание с случае какой-либо ошибки.

// Модуль Tiny RTS I2C
// PB6 - I2C1_SCL 
// PB7 - I2C1_SDA

#include "I2C.h"

volatile unsigned long Read_SR1;
volatile unsigned long Read_SR2;

// Инициализация I2C1. PB6 - I2C1_SCL. PB7 - I2C1_SDA.
void I2C1_Init(void){
	// Разрешение тактирования
	RCC_APB2ENR |= RCC_APB2ENR_IOPBEN; // Тактируем PortB
	RCC_APB1ENR |= RCC_APB1ENR_I2C1EN; // Тактируем I2C1
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN; // Активизация альтернативной функции
	// Настройка выходов I2C1 SDA - PB7 и SCL - PB6 в альтернативном режиме
	GPIOB_CRL &= ~0xFF000000; // Сброс битов настройки пинов PB7 и PB6
	GPIOB_CRL |= (GPIOB_CRL_MODE_50MHZ_PB6 |
								GPIOB_CRL_CNF_OUTOD_PB6	 |
								GPIOB_CRL_MODE_50MHZ_PB7 |
								GPIOB_CRL_CNF_OUTOD_PB7);
	// Сброс I2C1
	RCC_APB1RSTR |= RCC_APB1RSTR_I2C1RST;  // Установка исходное состояние 
	RCC_APB1RSTR &= ~RCC_APB1RSTR_I2C1RST; // всех регистров I2C1
	// Конфигурирование I2C1
	I2C1_CR2 &= ~0x3F;
	I2C1_CR2 |= I2C1_CR2_FREQ_24MHZ; // Частота тактирования модуля I2C1
	I2C1_CCR &= ~I2C1_CCR_FS_FAST;   // Режим Standart, 100 кГц
	I2C1_CCR &= ~I2C1_CCR_DUTY;	   	 // Этот параметр для режима Fast
//	I2C1_CCR &= ~0xFFF;
	I2C1_CCR |= I2C1_CCR_CCR;		 	 	 // 
	I2C1_TRISE |= I2C1_TRISE_TRISE;  // 
	I2C1_CR1 &= ~I2C1_CR1_SMBUS;		 // Установка режима "I2C"
	I2C1_CR1 |= I2C1_CR1_PE; 				 // Разрешение тактирования переферии
	I2C1_CR1 |= I2C1_CR1_ACK;				 //
}

// Ожидание отсутствия коммуникации I2C
int I2C_GetFlagSatusBUSY(void){
  while((I2C1_SR2 & I2C1_SR2_BUSY) == I2C1_SR2_BUSY){};
  return SUCCESS;
}

// Инициализации команды Start
void I2C_Init_Start(void){
  I2C1_CR1 |= I2C1_CR1_START;
}

// Проверка статуса команды Start, EV5
int I2C_CheckEventSBbit(void){
  while((I2C1_SR1 & I2C1_SR1_SB) == I2C1_SR1_SB){};
//  I2C1_SR1;
	return SUCCESS;
}

// Передача адреса ведомого
void I2C_Send7bitAddress(unsigned char SlaveAddress, unsigned char I2C_DirectionTransmitter){
  SlaveAddress = (SlaveAddress << 1) | I2C_DirectionTransmitter;
  I2C1_DR = SlaveAddress;
}

// Проверка статуса наличия ведомого по переданному адресу, EV6
// 1) Чтение SR1 (стр 573 RM0041)
// 2) Ожидание появления ADDR
// 3) Чтение SR2
int I2C_CheckEventADDR(void){
  while((I2C1_SR1 & I2C1_SR1_ADDR) == I2C1_SR1_ADDR){};
  Read_SR2 = I2C1_SR2;
  return SUCCESS;
}

// Проверка "Пустой буффер" TxE, EV8
int I2C_CheckEventTXE(void){
  while((I2C1_SR1 & I2C1_SR1_TXE) == I2C1_SR1_TXE);
  return SUCCESS;
}

// Передача байта
void I2C_SendByte(const unsigned char* buffer){
  I2C1_DR = *buffer; // Здесь пока не понятно!!!!!!!!!!!!!!!
}

// Проверка успешности передачи байта, флаг BTF
int I2C_CheckBTF(void){
  while((I2C1_SR1 & I2C1_SR1_BTF) == I2C1_SR1_BTF);
  return SUCCESS;
}

// Проверка условия для подачи команды Stop, EV8_2 (TXE = 1 и BTF = 1), EV8_2
int I2C_Check_TXEBTF(void){
	while(((I2C1_SR1 & I2C1_SR1_TXE) == I2C1_SR1_TXE) &&
			  ((I2C1_SR1 & I2C1_SR1_TXE) == I2C1_SR1_TXE));
	return SUCCESS;
}

// Инициализация команды Stop
void I2C_InitStop(void){
  I2C1_CR1 |= I2C1_CR1_STOP;
}

// Проверка статуса команды Stop, TxE = 0, BTF = 0
int I2C_CheckEventStop(void){
 // while(!I2C_CheckEventTXE & !I2C_CheckEventTXE){};
	return SUCCESS;
}

// Разрешение подтверждения - ACK
void I2C_AcknowledgeConfig(int PERMIT){
  if(PERMIT)
    I2C1_CR1 |= I2C1_CR1_ACK;
  else
    I2C1_CR1 &= ~I2C1_CR1_ACK;
}

// Управление флагом POS
void I2C_NACK_PositionConfig(int POSITION){
  if(NextByte)
    I2C1_CR1 |= I2C1_CR1_POS;
  else
    I2C1_CR1 &= ~I2C1_CR1_POS;
}

// Прием байта
unsigned char I2C_ReceivByte(void){
  unsigned char rbyte;
  rbyte = I2C1_DR;
  return rbyte;
}

// Получение флага RXNE
int I2C_GetFlagStatusRXNE(void){
  while((I2C1_SR1 & I2C1_SR1_RXNE) == I2C1_SR1_RXNE){};
  return SUCCESS;
}

// Передача I2C1
int I2C1_Transmit(const unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress){
	unsigned char I2C1_DirectionTransmitter = 0; // Передача
	if(nbyte > 0){
    I2C_GetFlagSatusBUSY();
// Инициализации команды Start
    I2C_Init_Start();
// Проверка статуса команды Start, EV5
    I2C_CheckEventSBbit();
// Передача адреса ведомого
    I2C_Send7bitAddress(SlaveAddress, I2C1_DirectionTransmitter);
// Проверка статуса наличия ведомого по переданному адресу, EV6
// 1) Чтение SR1 (стр 573 RM0041)
// 2) Ожидание появления ADDR
// 3) Чтение SR2
    I2C_CheckEventADDR();
// Проверка "Пустой буффер" TxE
//    I2C_CheckEventTXE();
// Передача первого байта
    I2C_SendByte(buffer++);
		while(--nbyte > 0){
// Проверка успешности передачи байта, флаг BTF
			I2C_CheckBTF();
//			I2C_CheckEventTXE();
			I2C_SendByte(buffer++);
		}
// Проверка условия для подачи команды Stop, EV8_2 (TXE = 1 и BTF = 1), EV8_2
		I2C_Check_TXEBTF();
// Инициализация команды Stop
//    I2C_CheckBTF();
		I2C_InitStop();
// Проверка статуса команды Stop, TxE = 0, BTF = 0
//    I2C_CheckEventStop();
  }
	
	return SUCCESS;
}

// Прием I2C1
int I2C1_Receive(unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress){
  unsigned char I2C1_DirectionTransmitter = 1; // Прием
  if(nbyte == 0)
    return SUCCESS;
// Ожидание бездействия I2C
//  I2C_GetFlagSatusBUSY();
// Ожидание Подтверждения и сброс POC
  I2C_AcknowledgeConfig(ENABLE);
  I2C_NACK_PositionConfig(CurrentByte);
// Инициализации команды Start
  I2C_Init_Start();
// Передача адреса ведомого, EV5
  I2C_Send7bitAddress(SlaveAddress, I2C1_DirectionTransmitter);
// EV6
  I2C_CheckEventADDR();
  if(nbyte == 1){ // Чтение 1-го байта
    // Сброс бита ACK
    I2C_AcknowledgeConfig(DISABLE);
    // EV6_1
    Read_SR1 = I2C1_SR1;
		Read_SR2 = I2C1_SR2; // Сброс бита ADDR  
    I2C_InitStop(); // Инициализация команды Stop
    // Прием байта EV7
    I2C_GetFlagStatusRXNE();
    *buffer++ = I2C_ReceivByte();
  }
  else if(nbyte == 2){ // Чтение 2-х байт
    // Установка флага POS
    I2C_NACK_PositionConfig(NextByte);
    // EV6_1
    Read_SR1 = I2C1_SR1;
		Read_SR2 = I2C1_SR2; // Сброс бита ADDR
    I2C_AcknowledgeConfig(DISABLE); // Сброс бита ACK
    // EV7_3, Ожидания BTF, остановка программы и прием байта дважды
//    I2C_CheckBTF();
    I2C_InitStop(); // Инициализация команды Stop
    *buffer++ = I2C_ReceivByte();
    *buffer++ = I2C_ReceivByte();
  }
  else{ // Чтение 3-х и более байтов
    Read_SR1 = I2C1_SR1;
		Read_SR2 = I2C1_SR2; // Сброс бита ADDR
    while(nbyte-- != 3){
      // EV7 - без гарантии, поэтому вместо RXNE ожидания BTF
      // Ожидание BTF
      I2C_CheckBTF();
      *buffer++ = I2C_ReceivByte();
    }
    // EV7_2
    I2C_CheckBTF();
		I2C_AcknowledgeConfig(DISABLE); // Сброс бита ACK
    *buffer++ = I2C_ReceivByte(); // Чтение N-2 байта
    I2C_InitStop(); // Инициализация команды Stop
    *buffer++ = I2C_ReceivByte(); // Чтение N-1 байта
    // Ожидание байта N
    I2C_GetFlagStatusRXNE();
    *buffer++ = I2C_ReceivByte(); // Чтение N байта
    
    nbyte = 0;
  }
  return SUCCESS;
}
