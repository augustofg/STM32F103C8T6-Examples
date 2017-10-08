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
 * Reads the analog channel 0 and
 * sends the value through the serial
 * port
 */
#include "stm32f103xb.h"
#include <stdlib.h>

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

/*
 * Delay function:
 * Uses the SysTick timer to wait for
 * an arbitrary time in microseconds.
 *
 * The clock source is assumed to be
 * 3MHz (HCLK/8)
 */
void delay_us(unsigned int time)
{
	/*
	 * Multiply the delay time by 3 (the clock 
	 * source is 3MHz and the time is specified
	 * in microseconds)
	 */
	SysTick->LOAD = time*3;

	/*
	 * Clears the current value and the count flag
	 */
	SysTick->VAL = 0;
	
	/*
	 * Waits until the count ends
	 */
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}

/*
 * Performs an analog to digital
 * conversion
 */
uint16_t ADC_Read(unsigned int channel)
{
	/*
	 * Selects the ADC channel
	 */
	ADC1->SQR1 = (1 << ADC_SQR1_L_Pos);
	ADC1->SQR3 = (channel & ADC_SQR3_SQ1_Msk);
	
	/*
	 * Starts the conversion
	 */
	ADC1->CR2 |= ADC_CR2_ADON;

	/*
	 * Waits until the conversion has
	 * finished
	 */
	while((ADC1->SR & ADC_SR_EOS) == 0);

	/*
	 * Returns the acquired value
	 */
	return ADC1->DR;
}

int main()
{
	char adstr[6];
	unsigned int ad_acc, i;

	/*
	 * Enable all Ports and Alternate Function clocks
	 * and the ADC clock
	 */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
	    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN |
		RCC_APB2ENR_ADC1EN;
	
	/*
	 * Disable JTAG and SWO (Free PB3, PB4 and PA15)
	 */
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

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
	 * Enable the SysTick Timer with
	 * the CPU clock divided by 8
	 */
	SysTick->CTRL = SysTick_CTRL_ENABLE;

	/*
	 * Enable the Analog to Digital
	 * converter
	 */
	ADC1->CR2 = ADC_CR2_ADON;

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
	 * Starts the ADC calibration
	 */
	ADC1->CR2 |= ADC_CR2_CAL;

	/*
	 * Waits until the calibration has
	 * finished
	 */
	while(ADC1->CR2 & ADC_CR2_CAL);

	while(1)
	{
		/*
		 * Accumulate 32 measures
		 */
		ad_acc = 0;
		for (i = 0; i < 32; i++)
		{
			ad_acc += ADC_Read(0);
		}

		/*
		 * Divide the accumulated measures
		 * by 32 and convert it to a string
		 */
		itoa(ad_acc >> 5, adstr, 10);

		/*
		 * Prints the analog value through
		 * the serial port
		 */
		Puts_USART1(adstr);
		Puts_USART1("\r\n");

		delay_us(500000); //500ms delay
	}
}
