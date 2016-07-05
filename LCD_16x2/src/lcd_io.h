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
 * I/O functions
 *
 * This is device specific code,
 * you may change this according
 * to the microcontroller and
 * pinout used
 */
#include "stm32f103xb.h"

/*
 * Sets the LCD RS (RS = 1)
 */
static inline void LCD_SET_RS()
{
	GPIOB->BSRR = 1;
}

/*
 * Sets the LCD Enable (E = 1)
 */
static inline void LCD_SET_E()
{
	GPIOB->BSRR = 2;
}

/*
 * Resets the LCD RS (RS = 0)
 */
static inline void LCD_RST_RS()
{
	GPIOB->BRR = 1;
}

/*
 * Resets the LCD Enable (E = 0)
 */
static inline void LCD_RST_E()
{
	GPIOB->BRR = 2;
}

/*
 * Configure the IOs
 */
static inline void LcdConfigIO()
{
	GPIOB->CRL = (GPIOB->CRL & 0xFF000000) | 0x00222222;
}

/*
 * Write a nibble to the LCD bus
 */
static inline void LcdPutNibble(char data)
{
	GPIOB->BRR = ~(data << 2) & 0b00111100;
	GPIOB->BSRR = (data << 2) & 0b00111100;
}
