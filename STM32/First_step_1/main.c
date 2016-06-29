/**
* Использование только адреса регистров.
* Keil 5.20
* V.0.00
* 12.05.2016
*/

int main()
{
//	unsigned long delay;
	volatile unsigned long i = 0;
	// 1) Тактируем PortA и PortC. Регистр RCC_ARB2ENR
	*(unsigned long *) 0x40021018 |= 0x14;
	// 2) Задержка для установления переходного процесса начала тактирования
	//	delay = *(unsigned long *) 0x20021018;
	i++; i++; i++;
	i = 0;
	// 3) При подаче питания или при перезагрузке порты устанавливаются
	// 		в GPIO и на вход. Ничего не надо здесь делать. Регистр GPIOA_CRL
	// 4) Устанавливаем биты PC8-9 в GPIO и на выход. Регистр GPIOC_CRH 
	*(unsigned long *) 0x40011004 &= (~0xFF);
	*(unsigned long *) 0x40011004 |= 0x22;
	// 5) LEDs off. Положительная логика
	*(unsigned long *) 0x4001100C &= (~0x0300);
	
	while(1){
		if(*((unsigned long *) 0x40010808) & 0x1) // Проверяем нажатие  кнопки. Положительная логика
			*(unsigned long *) 0x4001100C |= 0x0300; // LEDs on
		else
			*(unsigned long *) 0x4001100C &= (~0x0300); // LEDs off
	}
}
