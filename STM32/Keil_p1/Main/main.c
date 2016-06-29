#include "stm32f10x.h"

void InitAll(void)
{
   RCC->APB2ENR	|= RCC_APB2ENR_IOPAEN;

   GPIOA->CRL   |= GPIO_CRL_MODE3_0;
   return;
}

int main(void)
{
InitAll();

while(1)
	{
   	GPIOA->BSRR =GPIO_BSRR_BS3;
   	GPIOA->BSRR =GPIO_BSRR_BR3;
	}

}
