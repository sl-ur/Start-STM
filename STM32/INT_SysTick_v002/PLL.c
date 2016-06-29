// PLL.�
// ����������� ������� ��� ��������� ������� ���� � ������� PLL
// ����� 8 ���. PLL �� ������������ ������� 24 ���.
// 18.05.2016

#include "PLL.h"

// #define RCC_CR (*((unsigned long*) 0x40021000)) // ������� ���������� ������������
// ��������� ����� � �������� RCC_CR
#define HSEON 0x00010000 // ��������� �������� ������
#define PLLON 0x00100000 // ���������� PLL
#define PLLRDY 0x02000000 // ���� ���������� PLL
// #define RCC_CFGR (*((unsigned long*) 0x40021004)) // ������� ���������������� ������������
// ��������� ����� � �������� RCC_CFGR
#define PLLSRC_PREDIV1 0x0001000 // ����� ��������� ������������ PLL, 1 - �� PREDIV1, ������� �����
#define PLLXTPRE_div2 0x00020000 // ���������� ������������ PREDIV1 - 0, ��� ��������� /2 ��� 1
#define PLLMUL_x4 0x000C0000 // ����������� ��������� PLL x4 (0010)
#define SW_PLL 0x00000002 // ����� ��������� ���������� ������������, 10 - PLL

void PLL_Init(void)
{
	RCC_CR |= HSEON; // ��������� �������� ������ 8 ���
// ������ PLL �� ������� 24 ���
// 1) ������ �� ������ PLL
	RCC_CR &= ~PLLON;
// 2) ��������� ����������� ������������ PREDIV1
	RCC_CFGR &= ~PLLXTPRE_div2; // ������������ ��������
// 3) �������� ����������� PLL
	RCC_CFGR |= PLLSRC_PREDIV1; // �������� ������������ PLL ������� �����
// 4) ��������� ������������ ��������� PLL
	RCC_CFGR |= PLLMUL_x4; // x4
// 5) ��������� PPRE2 (APB2)
// 6) ��������� PPRE1 (APB1)
// 7) ��������� HPRE (AHB)
// 8) ���������� PLL
	RCC_CR |= PLLON;
// 9) �������� ���������� ������ PLL
	while((RCC_CR & PLLRDY) == 0) {}; // �������� ���������� PLL
// 10) ��������� ��������� ���������� ������������
	RCC_CFGR |= SW_PLL; // ��������� ������������ �� PLL
}
