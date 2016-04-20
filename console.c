/*
Copyright (C) 2014  Frank Duignan

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


#include "console.h"
#include <stdio.h> // contains definitions for putchar, getchar


void eputc(char c)
{
	putchar(c);
}
void eputs(const char *String)
{	
	while(*String)
	{
		eputc(*String);
		String++;
	}
}
char egetc()
{
	char input;
	input = getchar();	
	return input;
}
void printString(const char *String)
{
	eputs(String);
	eputs("\r\n");
}

char HexDigit(int Value)
{
	if ((Value >=0) && (Value < 10))
		return Value+'0';
	else 
		return Value-10 + 'A';
}
void printHex(unsigned int Number)
{
	// Output the number over the serial port as
	// as hexadecimal string.
	char TxString[9];
	int Index=8;
	TxString[Index]=0; // terminate the string
	Index--;
	while(Index >=0)
	{
		TxString[Index]=HexDigit(Number & 0x0f);
		Number = Number >> 4;
		Index--;
	}
	eputs(TxString);
}
