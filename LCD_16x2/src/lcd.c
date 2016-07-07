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

#include "lcd.h"
#include "lcd_io.h"

/*
 * Sends a nibble to the LCD
 */
void LcdSendNibble(char data)
{
	LcdPutNibble(data);
	LCD_SET_E();
	delay_us(20);
	LCD_RST_E();
	delay_us(20);
}

/*
 * Sends a command to the LCD
 */
void LcdSendCmd(char cmd)
{
	LcdSendNibble(cmd >> 4);
	LcdSendNibble(cmd);
}

/*
 * Write a character on the LCD
 */
void LcdWriteChar(char data)
{
	LCD_SET_RS();
	LcdSendNibble(data >> 4);
	LcdSendNibble(data);
	LCD_RST_RS();
}

/*
 * Initializes the LCD
 * 4-bit mode, 5x8 characters,
 * no cursor
 */
void LcdInit()
{
	LcdConfigIO();
	LCD_RST_E();
	LCD_RST_RS();
	delay_us(2000);
	LcdSendNibble(0x03);
	delay_us(5000);
	LcdSendNibble(0x03);
	delay_us(2000);
	LcdSendNibble(0x03);
	delay_us(2000);
	LcdSendNibble(0x02);
	delay_us(2000);
	LcdSendCmd(0x28);
	delay_us(2000);
	LcdSendCmd(0x0C);
	delay_us(2000);
	LcdSendCmd(0x01);
	delay_us(2000);
	LcdSendCmd(0x06);
	delay_us(2000);
}

/*
 * Write a null terminated string
 * to the LCD
 */
void LcdWriteString(const char *str)
{
	int i;
	for (i = 0; str[i] != 0; i++)
	{
		LcdWriteChar(str[i]);
	}
}

/*
 * Puts the LCD cursor to an
 * specified position
 */
void LcdSetAddress(char addr)
{
	LcdSendCmd(0x80 | addr);
}

/*
 * Clears the LCD and move
 * the cursor to the beginning
 */
void LcdClear()
{
	LcdSendCmd(0x01);
	delay_us(2000);
}
