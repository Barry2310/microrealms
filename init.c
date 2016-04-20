// Initialization and interrupt vectors for the LPC1114 microcontroller
// No claims are made as to the suitability of this code for any 
// purpose whatsoever.  Use at your own risk!
// Does not include initialization for C++ global/static objects
// so probably best to use this with C projects for now.
// Latest version of this code can be found by visiting
// http://eleceng.dit.ie/frank and follow the links

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

#include "lpc810.h"//sean change
void init(void);
void clock_init();
void Default_Handler(void);
void SysTick(void);
// The following are 'declared' in the linker script
extern unsigned char  INIT_DATA_VALUES;
extern unsigned char  INIT_DATA_START;
extern unsigned char  INIT_DATA_END;
extern unsigned char  BSS_START;
extern unsigned char  BSS_END;
void UART0_isr(void);
void SysTick(void);


// the section "vectors" is placed at the beginning of flash 
// by the linker script
const void * Vectors[] __attribute__((section(".vectors"))) ={
	(void *)(RAM_START+RAM_SIZE), 	/* Top of stack */ 
	init,   						/* Reset Handler */
	Default_Handler,				/* NMI */
	Default_Handler,				/* Hard Fault */
	0,	                			/* Reserved */
	0,            			 	    /* Reserved */
	0,                  			/* Reserved */
	0,                  			/* Reserved */
	0,                  			/* Reserved */
	0,                  			/* Reserved */
	0,                  			/* Reserved */
	Default_Handler,				/* SVC */
	0,                 				/* Reserved */
	0,                 				/* Reserved */
	Default_Handler,   				/* PendSV */
	SysTick, 						/* SysTick */		
/* External interrupt handlers follow */
	Default_Handler,				/* 0 SPI0_IRQ */
	Default_Handler,				/* 1 SPI1_IRQ */
	Default_Handler,				/* 2 RESERVED */
	UART0_isr,						/* 3 UART0_IRQ */
// The remaining interrupt vectors are not required for Realm
// removed to reclaim wasted space
#ifdef ALL_IRQS	
	Default_Handler,				/* 4 UART1_IRQ */
	Default_Handler,				/* 5 UART2_IRQ */
	Default_Handler,				/* 6 RESERVED */
	Default_Handler,				/* 7 RESERVED */
	Default_Handler,				/* 8 I2C0_IRQ */
	Default_Handler,				/* 9 SCT_IRQ */
	Default_Handler,				/* 10 MRT_IRQ */
	Default_Handler,				/* 11 CMP_IRQ */
	Default_Handler,				/* 12 WDT_IRQ */
	Default_Handler,				/* 16 RESERVED */
	Default_Handler,				/* 17 RESERVED */
	Default_Handler,				/* 18 RESERVED */
	Default_Handler,				/* 19 RESERVED */
	Default_Handler,				/* 20 RESERVED */
	Default_Handler,				/* 21 RESERVED */
	Default_Handler,				/* 22 RESERVED */
	Default_Handler,				/* 23 RESERVED */
	Default_Handler,				/* 24 PININT0_IRQ */
	Default_Handler,				/* 25 PININT1_IRQ */
	Default_Handler,				/* 26 PININT2_IRQ */
	Default_Handler,				/* 27 PININT3_IRQ */
	Default_Handler,				/* 28 PININT4_IRQ */
	Default_Handler,				/* 29 PININT5_IRQ */
	Default_Handler,				/* 30 PININT6_IRQ */
	Default_Handler,				/* 31 PININT7_IRQ */
#endif
};

void init()
{
// do global/static data initialization
	unsigned char *src;
	unsigned char *dest;
	unsigned len;
	src= &INIT_DATA_VALUES;
	dest= &INIT_DATA_START;
	len= &INIT_DATA_END-&INIT_DATA_START;
	while (len--)
		*dest++ = *src++;
// zero out the uninitialized global/static variables
	dest = &BSS_START;
	len = &BSS_END - &BSS_START;
	while (len--)
		*dest++=0;	
	main();
}

void Default_Handler()
{
	
	while(1);
}
