/**
* ������������� ������ ������ ���������.
* Keil 5.20
* V.0.00
* 12.05.2016
*/

int main()
{
//	unsigned long delay;
	volatile unsigned long i = 0;
	// 1) ��������� PortA � PortC. ������� RCC_ARB2ENR
	*(unsigned long *) 0x40021018 |= 0x14;
	// 2) �������� ��� ������������ ����������� �������� ������ ������������
	//	delay = *(unsigned long *) 0x20021018;
	i++; i++; i++;
	i = 0;
	// 3) ��� ������ ������� ��� ��� ������������ ����� ���������������
	// 		� GPIO � �� ����. ������ �� ���� ����� ������. ������� GPIOA_CRL
	// 4) ������������� ���� PC8-9 � GPIO � �� �����. ������� GPIOC_CRH 
	*(unsigned long *) 0x40011004 &= (~0xFF);
	*(unsigned long *) 0x40011004 |= 0x22;
	// 5) LEDs off. ������������� ������
	*(unsigned long *) 0x4001100C &= (~0x0300);
	
	while(1){
		if(*((unsigned long *) 0x40010808) & 0x1) // ��������� �������  ������. ������������� ������
			*(unsigned long *) 0x4001100C |= 0x0300; // LEDs on
		else
			*(unsigned long *) 0x4001100C &= (~0x0300); // LEDs off
	}
}
