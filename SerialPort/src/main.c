/*
  Copyright (c) 2015, Augusto Fraga Giachero
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

/*
 * Serial Port demo
 * Controls a LED through the serial port
 */
#include "stm32f103xb.h"

/*
 * Read a character through the serial port
 */
char Getc_USART1()
{
	while(!(USART1->SR & USART_SR_RXNE));
	return USART1->DR;
}

/*
 * Write a character through the serial port
 */
void Putc_USART1(char CharSend)
{
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = CharSend;
}

/*
 * Write a string through the serial port
 */
void Puts_USART1(const char * StringSend)
{
	uint32_t StrCount = 0;
	while (StringSend[StrCount])
	{
		Putc_USART1(StringSend[StrCount]);
		StrCount ++;
	}
}

int main()
{
	/*
	 * Enable all Ports and Alternate Function clocks
	 */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
	    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN;
	
	/*
	 * Disable JTAG and SWO (Free PB3, PB4 and PA15)
	 */
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

	/*
	 * Enable the PA1 as a digital output
	 */
	GPIOA->CRL = 0x00000020;

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
	 * AHB Prescaler: 2
	 * APB1 Prescaler: 1
	 * APB2 Prescaler: 1
	 * ADC Prescaler: 2
	 */
	RCC->CFGR = RCC_CFGR_PLLMULL6 | RCC_CFGR_PPRE2_DIV1 |
		RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_ADCPRE_DIV2 |
		RCC_CFGR_PLLSRC | RCC_CFGR_HPRE_DIV2 | RCC_CFGR_USBPRE;

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
	 * Now the CPU clock is 3*HSE (24MHz for
	 * an 8MHz oscillator)
	 */
	SystemCoreClockUpdate();

	/*
	 * Selects the PA9 as the USART1 RX
	 * Selects the PA10 as the USART1 TX
	 */
	GPIOA->CRH = (GPIOA->CRH & 0xFFFFF00F) | 0x000004B0;

	/*
	 * Enable the clock for the USART1
	 */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/*
	 * Adjusts the USART1 bit rate to
	 * 38400 bps (for an 24MHz input clock)
	 */
	USART1->BRR = 0x271;

	/*
	 * Enable the USART1 Transmitter and
	 * Receiver
	 */
	USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

	/*
	 * Turn off the LED
	 */
	GPIOA->BSRR = GPIO_BSRR_BS1;
	
	while(1)
	{
		/*
		 * Waits until a character is received
		 */
		switch (Getc_USART1())
		{
		case '0':
			GPIOA->BSRR = GPIO_BSRR_BS1; //PA1 = 1 (Led OFF)
			Puts_USART1("LED OFF\r\n");
			break;

		case '1':
			GPIOA->BSRR = GPIO_BSRR_BR1; //PA1 = 0 (Led ON)
			Puts_USART1("LED ON\r\n");
			break;
		}
	}
}
