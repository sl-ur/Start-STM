// I2C.h
// Карпов Вячеслав
// 09 июня 2016
// Данная версия только для мастера.
// В будущем сделать проверку на длительность ожидания статуса,
// что бы не впасть в бесконечное ожидание с случае какой-либо ошибки.
// И реализовать на прерываниях.
// Сделать выход 1 Гц в DS1307 и по внешнему прерываню запускать считывания часов

#include "def_f100x.h"

#define GPIOB_CRL_MODE_50MHZ_PB6	  0x03000000 // < 50 МГц
#define GPIOB_CRL_CNF_OUTOD_PB6			0x0C000000 // Выход с открытым коллектором (od)
#define GPIOB_CRL_MODE_50MHZ_PB7	  0x30000000 // < 50 МГц
#define GPIOB_CRL_CNF_OUTOD_PB7			0xC0000000 // Выход с открытым коллектором (od)
#define I2C1_CR1_ACK                0x400
#define I2C1_CR2_FREQ_24MHZ					0x18	 // 0b011000 Частота тактирования модуля I2C1
#define I2C1_CCR_FS_FAST						0x8000 // Fast Mode - '1', Standart Mode - '0'
#define I2C1_CCR_DUTY								0x4000 // 2 - '0', 16/9 - '1'
#define I2C1_CCR_CCR								113 	 // (Стандартный режим 100 кГц, тактирование модуля 24 МГц
#define I2C1_TRISE_TRISE						25  	 // Длительность переднего фронта в режиме Master
// Запись
#define I2C1_CR1_SMBUS							0x2 	 // '0' - I2C, '1' - SMBUS
#define I2C1_CR1_PE									0x1 	 // Разрешение переферии I2C
#define I2C1_CR1_START							0x100
#define I2C1_CR1_STOP               0x200
#define I2C1_SR1_SB                 0x1
#define I2C1_SR1_ADDR               0x2
#define I2C1_SR1_BTF                0x4
#define I2C1_SR1_TXE                0x80
#define I2C1_SR1_RXNE               0x40
#define I2C1_SR2_TRA                0x4 // '0' - Приемник, '1' - Передатчик
#define I2C1_SR2_BUSY               0x2 // '1' - идет связь, '0' - линия свободна
#define I2C1_SR2_MSL								0x1 // '0' - Ведомый, '1' - Ведущий
// Чтение
#define I2C1_CR1_ACK                0x400
#define I2C1_CR1_POS                0x800
#define I2C1_SR1_STOPF              0x10

#define SUCCESS											0
#define FAULT												1 
#define ENABLE                      1 // Разрешение подверждения ACK
#define DISABLE                     0 // Запрет подтверждения ACK
#define CurrentByte                 0 // POS
#define NextByte                    1 // POS

// Инициализация I2C1.
void I2C1_Init(void);
// Ожидание отсутствия коммуникации (бездействия) I2C
int I2C_GetFlagSatusBUSY(void);
// Инициализации команды Start
void I2C_Init_Start(void);
// Проверка статуса команды Start
int I2C_CheckEventSBbit(void);
// Передача адреса ведомого, EV5
void I2C_Send7bitAddress(unsigned char SlaveAddress, unsigned char I2C1_DirectionTransmitter);
// Проверка статуса наличия ведомого по переданному адресу
// 1) Чтение SR1 (стр 573 RM0041)
// 2) Ожидание появления ADDR
// 3) Чтение SR2
int I2C_CheckEventADDR(void);
// Проверка "Пустой буффер" TxE
int I2C_CheckEventTXE(void);
// Передача байта
void I2C_SendByte(const unsigned char* buffer);
// Проверка успешности передачи байта, флаг BTF
int I2C_CheckBTF(void);
// Проверка условия для подачи команды Stop, EV8_2 (TXE = 1 и BTF = 1)
int I2C_Check_TXEBTF(void);
// Инициализация команды Stop
void I2C_InitStop(void);
// Проверка статуса команды Stop, TxE = 0, BTF = 0
int I2C_CheckEventStop(void);
// Разрешение Подтверждения
void I2C_AcknowledgeConfig(int PERMIT);
// Управление флагом POS
void I2C_NACK_PositionConfig(int POSITION);
// Прием байта
unsigned char I2C_ReceivByte(void);
// Получение флага RXNE
int I2C_GetFlagStatusRXNE(void);

// Передача I2C1
int I2C1_Transmit(const unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress);

// Прием I2C1
int I2C1_Receive(unsigned char* buffer, unsigned long nbyte, unsigned char SlaveAddress);
