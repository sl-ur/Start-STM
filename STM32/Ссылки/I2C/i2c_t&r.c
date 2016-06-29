// http://blablacode.ru/mikrokontrollery/501
void HMC5883L_I2C_ByteWrite(u8 slaveAddr, u8* pBuffer, u8 WriteAddr)
{
    while(I2C_GetFlagStatus(HMC5883L_I2C, I2C_FLAG_BUSY));
    /* шлем старт */
    I2C_GenerateSTART(HMC5883L_I2C, ENABLE);
    /* ждем подтверждение */
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    /* шлем HMC5883 адрес */
    I2C_Send7bitAddress(HMC5883L_I2C, slaveAddr, I2C_Direction_Transmitter);
    /* ждем подтверждение */
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    /* шлем HMC5883L адрес регистра */
    I2C_SendData(HMC5883L_I2C, WriteAddr);
    /* ждем окончания передачи */
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /* шлем данные */
    I2C_SendData(HMC5883L_I2C, *pBuffer);
    /* ждем */
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /* шлем стоп */
    I2C_GenerateSTOP(HMC5883L_I2C, ENABLE);
}
 
void HMC5883L_I2C_BufferRead(u8 slaveAddr, u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
    /* ждем пока шина не освободится */
    while (I2C_GetFlagStatus(HMC5883L_I2C, I2C_FLAG_BUSY));
    /* шлем старт */
    I2C_GenerateSTART(HMC5883L_I2C, ENABLE);
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    /* шлем адрес */
    I2C_Send7bitAddress(HMC5883L_I2C, slaveAddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    /* Для очистки EV6 запускаем модуль снова */
    I2C_Cmd(HMC5883L_I2C, ENABLE);
    /* шлем HMC5883L адрес чтения */
    I2C_SendData(HMC5883L_I2C, ReadAddr);
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /* шлем старт второй раз */
    I2C_GenerateSTART(HMC5883L_I2C, ENABLE);
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    /* шлем HMC5883L адрес чтения */
    I2C_Send7bitAddress(HMC5883L_I2C, slaveAddr, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    /* Пока есть что читать */
    while (NumByteToRead)
    {
        if (NumByteToRead == 1)
        {
            I2C_AcknowledgeConfig(HMC5883L_I2C, DISABLE);
            I2C_GenerateSTOP(HMC5883L_I2C, ENABLE);
        }
        if (I2C_CheckEvent(HMC5883L_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            *pBuffer = I2C_ReceiveData(HMC5883L_I2C);
            pBuffer++;
            NumByteToRead--;
        }
    }
    I2C_AcknowledgeConfig(HMC5883L_I2C, ENABLE);
}