// INT_SysTick.c
// ѕереодические прерывани€ системным таймером SysTick
//  варцы 8 ћ√ц
// ¬нутреннее тактирование 24 ћ√ц
// ¬€чеслав  арпов
// 05.06.2016

#include "INT_SysTick.h"

// »нициализаци€ SysTick с разрешением прерывани€ и тактирование от системной шины.
#define SysTick_CTRL_SR    (*((volatile unsigned long *)0xE000E010))
#define SysTick_RELOAD_VR  (*((volatile unsigned long *)0xE000E014))
#define SysTick_CURRENT_VR (*((volatile unsigned long *)0xE000E018))
// #ifndef 
	
void INT_SysTick_Init(void){
  SysTick_CTRL_SR = 0;               // disable SysTick during setup
  SysTick_RELOAD_VR = 11989;         // значение дл€ 500 мкс - 1 к√ц (дл€ 24 ћ√ц, 41,7 нс)
  SysTick_CURRENT_VR = 0;            // value written to CURRENT clears
  SysTick_CTRL_SR = 0x00000007;      // enable SysTick with core clock
}


