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

/*
 * Play a chiptune
 *
 * The Timer 2 is used to generate
 * the note (Output Compare), while
 * the Timer 3 interrupts at the 
 * period of the note.
 */
#include "stm32f103xb.h"
#include "clock_cfg.h"
#include "tones.h"
#include "music.h"

/*
 * Global variable that points
 * to the note to be played
 */
volatile uint16_t NoteIndex = 0;

/*
 * PlayTone function:
 * Uses the Ouput Compare of
 * the Timer 2 to generate a
 * square wave signal of the
 * frequency of the note
 *
 * Tone:
 * 0 -> Silence
 * 1-88 -> Piano notes from A0
 * to C8
 */
void PlayTone(uint8_t Tone)
{
	/*
	 * Disable the timer 2 counting
	 */
	TIM2->CR1 &= ~(TIM_CR1_CEN);
	if (Tone > 0)
	{
		/*
		 * Clears the count value and
		 * loads the timer parameters to
		 * the specified tone (preescaler,
		 * auto-reload and compare value).
		 */
		TIM2->CNT = 0;
		TIM2->ARR = TIM2->CCR3 = TonePeriods[Tone - 1].count;
		TIM2->PSC = TonePeriods[Tone - 1].div;

		/*
		 * Enable the timer 2 counter
		 */
		TIM2->CR1 |= TIM_CR1_CEN;
	}
}

/*
 * Timer 3 interrupt
 */
void TIM3_IRQHandler()
{
	/*
	 * Test for the Update Event
	 * (CNT Reload)
	 */
	if(TIM3->SR & TIM_SR_UIF)
	{
		/*
		 * Reset the counter and load
		 * the note period
		 */
		TIM3->ARR = Music[NoteIndex].duration;
		TIM3->CNT = 0;

		/*
		 * Play the note
		 */
		PlayTone(Music[NoteIndex].tone);

		/*
		 * Increments the note index
		 */
		NoteIndex++;

		if (!(NoteIndex < ((sizeof(Music) / sizeof(Music[0])))))
		{
			//TIM3->CR1 &= ~(TIM_CR1_CEN);
			NoteIndex = 0;
		}

		/*
		 * Clears the interrupt event
		 */
		TIM3->SR &= ~(TIM_SR_UIF);
	}
}

void delay_us(unsigned int time)
{
	/*
	 * Multiply the delay time by 6 (the clock 
	 * source is 6MHz and the time is specified
	 * in microseconds)
	 */
	SysTick->LOAD = time*6;

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
	 * Do all the clock configuration
	 */
	ClockCFG();

	/*
	 * Disable JTAG and SWO (Free PB3, PB4 and PA15)
	 */
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

	/*
	 * Enable the timer clock
	 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN;

	/*
	 * Enable the TIM2_CH3
	 */
	GPIOA->CRL =  (GPIOA->CRL & ~(GPIO_CRL_MODE2_Msk | GPIO_CRL_CNF2_Msk)) |
		GPIO_CRL_MODE2_1 | GPIO_CRL_CNF2_1;

	/*
	 * Toggle on successful compare
	 */
	TIM2->CCMR2 = TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1;

	/*
	 * Enable compare channel 3 of the
	 * timer 2
	 */
	TIM2->CCER |= TIM_CCER_CC3E;
	
	/*
	 * 48000 preescaler
	 */
	TIM3->PSC = 47999;

	/*
	 * Enable the Update
	 */
	TIM3->DIER |= TIM_DIER_UIE;

	/*
	 * The timer 3 will interrupt after
	 * 1ms to start play the music
	 */
	TIM3->ARR = 1;

	/*
	 * Enable the timer 3 interrupt
	 */
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 2);
	__enable_irq();

	/*
	 * Enable timer 3 counter
	 */
	TIM3->CR1 = TIM_CR1_CEN;
	
	/*
	 * Infinite loop
	 */
	while(1)
	{
		
	}
}
