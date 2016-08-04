/*
  Copyright (c) 2016, Augusto Fraga Giachero
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL AUGUSTO FRAGA GIACHERO BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "stm32f103xb.h"

/*
 * Function to configure the clock
 * distribution
 */
int ClockCFG()
{
	/*
	 * Enable all Ports and Alternate Function clocks
	 */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
	    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN;

	/*
	 * Set flash memory latency to 1 to
	 * allow the cpu to operate at 48Mhz
	 */
	FLASH->ACR |= FLASH_ACR_LATENCY_1 | FLASH_ACR_PRFTBE;

	/*
	 * Enable the high speed external
	 * oscillator
	 */
	RCC->CR |= RCC_CR_HSEON;
	
	/*
	 * Waits until the external oscillator
	 * is ready
	 */
	while((RCC->CR & RCC_CR_HSERDY) == 0);

	/*
	 * PLL Source: HSE
	 * PLL Multiply Factor: 6
	 * USB Prescaler: 1
	 * AHB Prescaler: 1
	 * APB1 Prescaler: 2
	 * APB2 Prescaler: 1
	 * ADC Prescaler: 4
	 */
	RCC->CFGR = RCC_CFGR_PLLMULL6 | RCC_CFGR_PPRE2_DIV1 |
		RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_ADCPRE_DIV2 |
		RCC_CFGR_PLLSRC | RCC_CFGR_HPRE_DIV1 | RCC_CFGR_USBPRE |
		RCC_CFGR_ADCPRE_DIV4;

	/*
	 * Enable the PLL
	 */
	RCC->CR |= RCC_CR_PLLON;

	/*
	 * Waits until the PLL is ready
	 */
	while((RCC->CR & RCC_CR_PLLRDY) == 0);

	/*
	 * Switch the System clock to the PLL
	 */
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_PLL) | RCC_CFGR_SW_PLL;

	/*
	 * Updates the SystemCoreClock variable
	 * Now the CPU clock is 6*HSE (48MHz for
	 * an 8MHz oscillator)
	 */
	SystemCoreClockUpdate();

	/*
	 * Enable the SysTick Timer with
	 * the CPU clock divided by 8
	 */
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
	
	return 0;
}
