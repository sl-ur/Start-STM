// http://badembed.ru/primery-ispolzovaniya-stdperiph-i2c/
// В самом начале настраиваем I2С и прерывания для него:

GPIO_InitTypeDef GPIO_InitStruct;
I2C_InitTypeDef I2C_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOB, &GPIO_InitStruct);

GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

NVIC_InitStruct.NVIC_IRQChannel = I2C1_EV_IRQn;
NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStruct);

NVIC_InitStruct.NVIC_IRQChannel = I2C1_ER_IRQn;
NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStruct);

I2C_DeInit(I2C1);

I2C_InitStruct.I2C_ClockSpeed = 100000;
I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
I2C_InitStruct.I2C_OwnAddress1 = 0xaa;
I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

I2C1->CR1 |= I2C_CR1_ACK;

I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);

I2C_Init(I2C1, &I2C_InitStruct);
I2C_Cmd(I2C1, ENABLE);

// Прерывание I2C_IT_BUF — включаем только перед самой передачей к мастеру — 
// так надо, по алгоритму работы c hardware периферией STM32 (его нужно будет 
// отключать и после передачи последнего байта по TXE, чтобы после сработало 
// прерывание BTF  по которому ставим stop условие).

// Когда хотим передавать — master формирует start условие:

void i2c_send(void)
{
    // ждем освобождения шины
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    // вкл. прерывания I2C_IT_BUF и формируем START условие
    I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
    I2C_GenerateSTART(I2C1, ENABLE);
}

// Далее вся работа происходит в прерываниях:

// master
if( CHECK_FLAG(I2C1->SR2, I2C_SR2_MSL) )
{
    // если шина не занята (start условие сработало)
    if(I2C_GetITStatus(I2C1, I2C_IT_SB)== SET)
    {
        I2C_Send7bitAddress(I2C1, 0xaa, I2C_Direction_Transmitter);
        I2C_ITConfig(I2C1, I2C_IT_BUF , ENABLE);
    }

    // передали байт
    if( (I2C1->SR2 & I2C_SR2_TRA) == I2C_SR2_TRA)
    {
        // TXE установлен, BTF установлен (только в этот момент можно генерировать STOP)
        if( I2C_GetITStatus(I2C1, I2C_IT_TXE) == SET &&
            I2C_GetITStatus(I2C1, I2C_IT_BTF) == SET )
        {
            I2C_GenerateSTOP(I2C1, ENABLE);
        }

        // TXE установлен, BTF нет          
        if( I2C_GetITStatus(I2C1, I2C_IT_TXE) == SET &&
            I2C_GetITStatus(I2C1, I2C_IT_BTF) == RESET )
        {
            i2c_trans_1_byte_2_port();
        }

        // TXE не установлен, BTF установлен
        if( I2C_GetITStatus(I2C1, I2C_IT_TXE) == RESET &&
            I2C_GetITStatus(I2C1, I2C_IT_BTF) == SET )
        {
            i2c_trans_1_byte_2_port();
        }
    }

    // приняли байт
    else
    {
        // в данном примере master только передает  
    }
}

// slave
else
{
    // приняли свой адрес (очистака I2C_IT_ADDR чтением SR1 и SR2)
    if(I2C_GetITStatus(I2C1, I2C_IT_ADDR) == SET)
    {
        I2C1->SR1;
        I2C1->SR2;
        // включаем прерывание I2C_IT_BUF - вначале было отключено
        I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
    }

    // стоп (конец приема - stop условие)
    else if(I2C_GetITStatus(I2C1, I2C_IT_STOPF) == SET)
    {
        // снова отключение I2C_IT_BUF
        I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);

        // сбрасываем I2C_IT_STOPF чтением SR1 и записью в CR1 (из примера от ST) 
        I2C1->SR1;
        I2C1->CR1 |= I2C_CR1_PE;
    }

    else
    {
        // передали байт
        if((I2C1->SR2 & I2C_SR2_TRA) == I2C_SR2_TRA)
        {
            // в данном примере slave только принимает
        }

        // приняли байт
        else
        {
            // RXNE установлен, BTF нет
            if( I2C_GetITStatus(I2C1, I2C_IT_RXNE) == SET &&                    
                I2C_GetITStatus(I2C1, I2C_IT_BTF) == RESET )
            {
                i2c_rec_1_byte_2_port();
            }

            // BTF установлен
            else if( I2C_GetITStatus(I2C1, I2C_IT_BTF) == SET)

            {
                i2c_rec_1_byte_2_port();
            }
        }
    }
}

// В функции приема i2c_rec_1_byte_2_port просто копируем данные из буфера I2C:

data[i] = I2C1->DR;
i++;

// В функции передачи i2c_trans_1_byte_2_port надо сразу после того как положили 
// последний байт на передачу в регистр I2C отключить прерывание от I2C_IT_BUF:

I2C1->DR = data[i];
i--;
if(i == 0)
    I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);

// Если это прерывание в данный момент не отключить не произойдет прерывание 
// I2C_IT_BUF, а по нему мы шлем stop условие.