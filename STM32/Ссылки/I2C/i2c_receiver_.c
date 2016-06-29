// http://microtechnics.ru/stm32-ispolzovanie-i2c/
//============================================================================
// Инициализация I2C в режиме Слейва.
//============================================================================
//I2C с кросс платой.
#define PORT_I2C_OUT GPIOB
#define I2C_SCL_OUT GPIO_Pin_10
#define I2C_SDA_OUT GPIO_Pin_11 //I2C интерфейс с кросс-платой.
#define I2C_READY GPIO_Pin_12 //флаг готовности I2C с буфера.
void Init_I2C_Slave(void)
{
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

//ЦИФРОВЫЕ ВХОДЫ:
GPIO_Init_I2C_Slave.GPIO_Mode = GPIO_Mode_IN; //IN -вход цифровой //OUT -выход цифровой //AN-аналоговый //AF-альтернативные функции
GPIO_Init_I2C_Slave.GPIO_OType = GPIO_OType_PP ; //PP-с полным размахом //OD -открытый коллектор
GPIO_Init_I2C_Slave.GPIO_Speed = GPIO_Speed_2MHz; //100MHz 50MHz 25MHz 2MHz
GPIO_Init_I2C_Slave.GPIO_PuPd = GPIO_PuPd_NOPULL; //NOPULL-подтяжки нет // UP-подтяжка к плюсу //DOWN-подтяжка к земле
GPIO_Init_I2C_Slave.GPIO_Pin = I2C_READY;
GPIO_Init(PORT_I2C_OUT, &GPIO_Init_I2C_Slave);

//конфигурация выводов PB10-SCL, PB11-SDA.
GPIO_Init_I2C_Slave.GPIO_OType = GPIO_OType_OD;
GPIO_Init_I2C_Slave.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init_I2C_Slave.GPIO_Mode = GPIO_Mode_AF;
GPIO_Init_I2C_Slave.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_Init_I2C_Slave.GPIO_Pin = I2C_SCL_OUT | I2C_SDA_OUT;
GPIO_Init(PORT_I2C_OUT, &GPIO_Init_I2C_Slave);

GPIO_PinAFConfig(PORT_I2C_OUT, GPIO_PinSource10, GPIO_AF_I2C2); //указываем альтернативные функции для пинов.
GPIO_PinAFConfig(PORT_I2C_OUT, GPIO_PinSource11, GPIO_AF_I2C2);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE); // затактировали I2C2

//I2C_InitTypeDef I2C_InitStructure;
I2C_Init_Slave.I2C_ClockSpeed = 50000; //скорость 100 КГц.
I2C_Init_Slave.I2C_Mode = I2C_Mode_I2C;
I2C_Init_Slave.I2C_DutyCycle = I2C_DutyCycle_2;
I2C_Init_Slave.I2C_OwnAddress1 = (GA_Adress << 1); //адрес нашей платы (задаем географическую адресацию).
I2C_Init_Slave.I2C_Ack = I2C_Ack_Enable;
I2C_Init_Slave.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
I2C_Init(I2C2, &I2C_Init_Slave);

//Configure the I2C event priority//
NVIC_Init_I2C.NVIC_IRQChannel = I2C2_EV_IRQn;
NVIC_Init_I2C.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_Init_I2C.NVIC_IRQChannelSubPriority = 0;
NVIC_Init_I2C.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_Init_I2C);

//Configure I2C error interrupt to have the higher priority.
NVIC_Init_I2C.NVIC_IRQChannel = I2C2_ER_IRQn;
NVIC_Init(&NVIC_Init_I2C);

I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);
I2C_ITConfig(I2C2, I2C_IT_EVT, ENABLE);
I2C_ITConfig(I2C2, I2C_IT_BUF, ENABLE);

I2C_Cmd(I2C2, ENABLE); // включили I2C2
I2C_AcknowledgeConfig(I2C2, ENABLE);
}

//===============================================================================
//===============================================================================
// ПРЕРЫВАНИЯ
//===============================================================================
//===============================================================================

//=============================================================================
// Прерывания по событию I2C2
//=============================================================================
//процедура записи у мастера: старт — посылаем адрес устройства — посылаем адрес регистра — посылаем новые данные — стоп.
//процедура чтения у мастера: старт — посылаем адрес устройства — посылаем адрес регистра — второй старт- посылаем адрес устройства — читаем данные — стоп.

//=========================== Slave Events ================================
void I2C2_EV_IRQHandler(void)
{
uint32_t event = 0;
event = I2C_GetLastEvent(I2C2); //возвращаем переменной оба статус регистра.

switch (event)
{
//EV1: ADDR=1, cleared by reading SR1 followed by reading SR2
//EV3_1: TxE=1, shift register empty,data register empty, write Data1 in DR.
case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED: //совпадение адреса.
Tx_Idx = 0;
I2C_ReadRegister(I2C2,I2C_Register_SR1); //убираем флаг ADDR для этого читаем SR1 и SR2. (cleared by software sequence).
I2C_ReadRegister(I2C2,I2C_Register_SR2);
I2C_SendData(I2C2, TxBuffer[Tx_Idx]); //передаем данные мастеру.
break;

//EV3: TxE=1, shift register not empty,data register empty, cleared by writing DR.
case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:
Tx_Idx++;
I2C_SendData(I2C2, TxBuffer[Tx_Idx]);
//сделать ограничение на передачу: выдавать ошибку если мастер просит передачи, а байты уже закончились.
break;

//EV1: ADDR=1, cleared by reading SR1 followed by reading SR2.
case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED : //совпадение адреса.
Rx_Idx = 0x00;
I2C_ReadRegister(I2C2,I2C_Register_SR1); //убираем флаг ADDR для этого читаем SR1 и SR2. (cleared by software sequence).
I2C_ReadRegister(I2C2,I2C_Register_SR2);
break;

//EV2: RxNE=1, cleared by reading DR register.
case I2C_EVENT_SLAVE_BYTE_RECEIVED: //читаем данные от мастера.
RxBuffer[Rx_Idx++] = I2C_ReceiveData(I2C2);
break;

//EV4: STOPF=1, cleared by reading SR1 register followed by writing to the CR1 register.
case I2C_EVENT_SLAVE_STOP_DETECTED:
I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF); //STOPF (STOP detection) is cleared by software sequence: a read operation to I2C_SR1 register (I2C_GetFlagStatus())
I2C_Cmd(I2C2, ENABLE); //followed by a write operation to I2C_CR1 register (I2C_Cmd() to re-enable the I2C peripheral).
break;

default:
break;
}
}

//=========================== Slave Erors =================================
//Отслеживаем NACK.
void I2C2_ER_IRQHandler(void)
{
//EV3_2: AF=1, AF is cleared by writing '0' in AF bit of SR1 register.
if (I2C_GetITStatus(I2C2, I2C_IT_AF)) I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
}
Ответить ↓	