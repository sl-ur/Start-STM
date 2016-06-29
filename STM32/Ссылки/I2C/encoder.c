// http://mycontroller.ru/old_site/stm32-podklyuchaem-enkoder/#more-2948
// Энкодер я подключил к входам таймера TIM3. Для начала нужно выполнить настройку выводов:

RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //включить тактирование порта
//Вывод GPIOB.6
GPIOB->CRL &= ~GPIO_CRL_MODE6;      //очистить разряды MODE
GPIOB->CRL &= ~GPIO_CRL_CNF6;       //очистить разряды CNF
GPIOB->CRL |=  GPIO_CRL_CNF6_1;     //дискретный вход с подтягивающим резистором
GPIOB->BSRR =  GPIO_BSRR_BS6;       //подтяжка к плюсу
//Вывод GPIOB.7
GPIOB->CRL &= ~GPIO_CRL_MODE7;      //очистить разряды MODE
GPIOB->CRL &= ~GPIO_CRL_CNF7;       //очистить разряды CNF
GPIOB->CRL |=  GPIO_CRL_CNF7_1;     //дискретный вход с подтягивающим резистором
GPIOB->BSRR =  GPIO_BSRR_BS7;       //подтяжка к плюсу

// Настройка таймера выполняется следующем образом:

RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;                   //тактирование таймера
TIM3->CCMR1   = TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;  //полярность сигнала для каждого входа
TIM3->CCER    = TIM_CCER_CC1P | TIM_CCER_CC2P;
TIM3->SMCR    = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;      //режим энкодера
TIM3->ARR     = 1023;                                 //значение, до которого считает CNT
TIM3->CR1     = TIM_CR1_CEN;                          //включаем таймер