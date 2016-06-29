// http://catethysis.ru/i2c-restart-error-handling/
void delay()
{
	for(volatile uint32_t del = 0; del<250000; del++);
}

typedef enum { EVENT, FLAG } OCCASION;

ErrorStatus wait_for(OCCASION occasion, uint32_t I2C_FLAG_OR_EVENT)
{
	uint16_t ticks = 200; // таймаут - 200 проверок события или флага. нужно подбирать это значение.
	while(	(occasion == FLAG ? I2C_GetFlagStatus(I2C1, I2C_FLAG_OR_EVENT) : !I2C_CheckEvent(I2C1, I2C_FLAG_OR_EVENT))
		  	&&
			ticks) // а не вышел ли таймаут?
		ticks--;
	
	if(ticks == 0)
	/*
	вышли из while по таймауту, это нестандартная ситуация - значит, произошла ошибка.
	честно говоря, сейчас не очень интересно что за ошибка -  просто вернём значение ERROR.
	при желании можно спросить текущий статус в регистрах статуса SR1 и SR2.
	*/
	{
		I2C_DeInit(I2C1);
			delay();
		I2C_init();
			delay();
		return ERROR; // Ошибка в модуле I2C, теперь нужно прервать текущие операции
	}
	return SUCCESS;
}

ErrorStatus I2C_single_write(uint8_t HW_address, uint8_t data)
{
	/*
	переходим к следующему действию только если предыдущее выполнено со статусом SUCCESS
	как только в любом действии получаем ERROR - сразу выходим из всей функции.
	*/
	I2C_GenerateSTART(I2C1, ENABLE);
		if(wait_for(EVENT, I2C_EVENT_MASTER_MODE_SELECT) == ERROR) return ERROR;
		// раньше было while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, HW_address, I2C_Direction_Transmitter);
		if(wait_for(EVENT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) return ERROR;
	I2C_SendData(I2C1, data);
		if(wait_for(EVENT, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR) return ERROR;
	I2C_GenerateSTOP(I2C1, ENABLE);
		if(wait_for(FLAG, I2C_FLAG_BUSY) == ERROR) return ERROR;
	return SUCCESS;
}