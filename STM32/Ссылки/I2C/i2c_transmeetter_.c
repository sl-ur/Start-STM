// http://microtechnics.ru/stm32-ispolzovanie-i2c/
// master stm32f4
// Для МАСТЕРА!!!!!!!!!!
 
**** WRITE_I2C( (1<<1), 0x20, 0xAA); //записываем в устройство с адресом 0х01 в регистр с адресом 0х20 байт 0хАА.
**** WRITE_MULTI_I2C( (2<<1), 0x70, 5); //записываем в устройство с адресом 0х02 в регистры с адресом 0х70…0х74 байты, записанные в массиве TxBuffer_I2C.
**** Data_I2C = READ_I2C( (1<<1), 0x26);//прочитаем из устройства с адресом 0х01 регистр с адресом 0х26.
**** READ_MULTI_I2C( (2< 10));
if (Timeout 10));
if (Timeout max_data_Tx) ) return 0; //если записали отправить 0 байт или больше максимального, то выход.

//ждем пока шина освободится.
Timeout = Timeout_for_I2C1;
while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && (Timeout— > 10));
if (Timeout<=10) {return 0; }///ошибка связи

//генерируем старт-бит.
I2C_GenerateSTART(I2C1, ENABLE);
//ждем флага "Start condition generated". (EV5).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
{ if(Timeout!=0) Timeout—; else {return 0;} }

//посылаем адрес устройства подчиненному.
I2C_Send7bitAddress(I2C1, Slave_addr, I2C_Direction_Transmitter);
//ждем флага "Address sent". (EV6).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR))
{ if(Timeout != 0) Timeout—; else {I2C_GenerateSTOP(I2C1, ENABLE); return 0;} }

I2C_ReadRegister(I2C1,I2C_Register_SR1); //убираем флаг для этого читаем SR1 и SR2. (cleared by software sequence).
I2C_ReadRegister(I2C1,I2C_Register_SR2);

//ждем флага "Data register empty". (EV8_1).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
{ if(Timeout != 0) Timeout—; else {return 0;} }

//посылаем адрес регистра подчиненному.
I2C_SendData(I2C1, Reg_addr);
//ждем флага "Data register empty". (EV8).
Timeout = Timeout_for_I2C1;
while ((!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF)))
{ if(Timeout != 0) Timeout—; else {return 0;} }

//посылаем новые данные подчиненному.
for(cnt_i2c = 0; cnt_i2c max_data_Rx) ) return 0; //если записали прочитать 0 байт или больше максимального, то выход.

//ждем пока шина освободится.
Timeout = Timeout_for_I2C1;
while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)&& (Timeout— > 10));
if (Timeout<=10) {return 0;}///ошибка связи

//генерируем старт-бит.
I2C_GenerateSTART(I2C1, ENABLE);
//ждем флага "Start condition generated". (EV5).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
{ if(Timeout != 0) Timeout—; else {return 0;} }

I2C_AcknowledgeConfig(I2C1, ENABLE); //ACK выкл.

//посылаем адрес устройства подчиненному.
I2C_Send7bitAddress(I2C1, Slave_addr, I2C_Direction_Transmitter);
//ждем флага "Address sent". (EV6).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR))
{ if(Timeout != 0) Timeout—; else {I2C_GenerateSTOP(I2C1, ENABLE); I2C_AcknowledgeConfig(I2C1, ENABLE); return 0;} }

I2C_ReadRegister(I2C1,I2C_Register_SR1); //убираем флаг для этого читаем SR1 и SR2. (cleared by software sequence).
I2C_ReadRegister(I2C1,I2C_Register_SR2);

//ждем флага "Data register empty". (EV8_1).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE))
{ if(Timeout != 0) Timeout—; else {return 0;} }

//посылаем адрес регистра подчиненному.
I2C_SendData(I2C1, Reg_addr);
//ждем флага "Data register empty". (EV8).
Timeout = Timeout_for_I2C1;
while ((!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF)))
{ if(Timeout != 0) Timeout—; else {return 0;} }

//генерируем старт-бит второй раз.
I2C_GenerateSTART(I2C1, ENABLE);
//ждем флага "Start condition generated". (EV5).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
{ if(Timeout != 0) Timeout—; else {return 0;} }

//посылаем адрес устройства подчиненному.
I2C_Send7bitAddress(I2C1, Slave_addr, I2C_Direction_Receiver);
//ждем флага "Address sent". (EV6).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR))
{ if(Timeout != 0) Timeout—; else {return 0;} }

I2C_ReadRegister(I2C1,I2C_Register_SR1); //убираем флаг для этого читаем SR1 и SR2. (cleared by software sequence).
I2C_ReadRegister(I2C1,I2C_Register_SR2);

for(cnt_i2c = 0; cnt_i2c = Cnt)
{
// ACK disable
I2C_AcknowledgeConfig(I2C1, DISABLE);
//генерируем стоп-бит.
I2C_GenerateSTOP(I2C1, ENABLE);
}
//ждем флага «Data register not empty». (EV7).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE))
{ if(Timeout != 0) Timeout—;
else {return 0;} }
//чтение данных.
RxBuffer_I2C[cnt_i2c] = I2C_ReceiveData(I2C1);
}

//ждем флага «Data register not empty». (EV7).
Timeout = Timeout_for_I2C1;
while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE))
{ if(Timeout != 0) Timeout—; else {return 0;} }

//генерируем стоп-бит.
//I2C_GenerateSTOP(I2C1, ENABLE);

I2C_AcknowledgeConfig(I2C1, ENABLE); //ACK вкл.

return tmp;
}

I2C slave stm32f4
Для СЛЕЙВА!!!!!!!!!!

/*** Функции передачи и чтения информации по I2C.
**** все действия происходят по прерываниям.
****
*/

#include «stm32f4xx.h»
#include «stm32f4xx_i2c.h»
#include «I2C_slave.h»

GPIO_InitTypeDef GPIO_Init_I2C_Slave;
I2C_InitTypeDef I2C_Init_Slave;
NVIC_InitTypeDef NVIC_Init_I2C;

extern uint8_t GA_Adress; //адрес нашего устройства.

extern uint8_t TxBuffer[10]; //массив для передачи данных мастеру.
extern uint8_t Tx_Idx;
extern uint8_t RxBuffer[10]; //массив для приема данных от мастера.
extern uint8_t Rx_Idx;