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
 * LCD 16x2 demo
 */
#include "stm32f103xb.h"
#include "lcd.h"

/*
 * Delay function:
 * Uses the SysTick timer to wait for
 * an abitrary time in microseconds.
 *
 * The clock source is assumed to be
 * the internal 8MHz RC oscilator
 */
void delay_us(unsigned int time)
{
	/*
	 * Multiply the delay time by 8 (source is
	 * 8MHz and the time is specified in us)
	 */
	SysTick->LOAD = time;

	/*
	 * Clears the current value and the count flag
	 */
	SysTick->VAL = 0;

	/*
	 * Waits until the count ends
	 */
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}

int main()
{
	/*
	 * Enable all ports and Alternate Function clocks
	 */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
	    RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_AFIOEN;
	
	/*
	 * Disable JTAG and SWO (Free PB3, PB4 and PA15)
	 */
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

	/*
	 * Enable the SysTick Timer
	 */
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;

	/*
	 * 100ms delay to ensure that the
	 * LCD display is properly powered on 
	 */
	delay_us(100000);

	/*
	 * Initializes the LCD
	 */
	LcdInit();

	/*
	 * Write a string to the LCD
	 * display
	 */
	LcdWriteString(" ARM Cortex M3  ");
	while(1)
	{
		/*
		 * Move the cursor to the 2nd
		 * line of the LCD
		 */
		LcdSetAddress(0x40);

		/*
		 * Write a string to the LCD
		 * display
		 */
		LcdWriteString(" STM32F103C8T6 ");
		
		delay_us(2000000); // 2s delay

		/*
		 * Move the cursor to the 2nd
		 * line of the LCD
		 */
		LcdSetAddress(0x40);

		/*
		 * Write a string to the LCD
		 * display
		 */
		LcdWriteString("     @ 8MHz    ");

		delay_us(2000000); // 2s delay
	}
}
