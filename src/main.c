#include "stm32f103xb.h"

int main()
{
	/* Enable all ports and Alternate Function clocks */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
	    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN;
	/* Disable JNTRST and SWO (Free PB3 and PB4) */
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_NOJNTRST;
	while(1)
	{
		
	}
	return 0;
}
