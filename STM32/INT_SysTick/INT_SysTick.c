// INT_SysTick.c
// ������������� ���������� ��������� �������� SysTick
// ������ 8 ���
// ���������� ������������ 24 ���
// �������� ������
// 05.06.2016

#include "INT_SysTick.h"

// ������������� SysTick � ����������� ���������� � ������������ �� ��������� ����.
#define SysTick_CTRL_SR    (*((volatile unsigned long *)0xE000E010))
#define SysTick_RELOAD_VR  (*((volatile unsigned long *)0xE000E014))
#define SysTick_CURRENT_VR (*((volatile unsigned long *)0xE000E018))
// #ifndef 
	
void INT_SysTick_Init(void){
  SysTick_CTRL_SR = 0;               // disable SysTick during setup
  SysTick_RELOAD_VR = 11989;         // �������� ��� 500 ��� - 1 ��� (��� 24 ���, 41,7 ��)
  SysTick_CURRENT_VR = 0;            // value written to CURRENT clears
  SysTick_CTRL_SR = 0x00000007;      // enable SysTick with core clock
}


