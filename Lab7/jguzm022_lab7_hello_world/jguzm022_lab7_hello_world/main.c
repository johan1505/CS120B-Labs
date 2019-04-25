/*
 * jguzm022_lab7_hello_world.c
 *
 * Created: 4/24/2019 8:04:22 AM
 * Author : joan1
 */ 

#include <avr/io.h>
#include "io.c"

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	// Initializes the LCD display
	LCD_init();
	
	// Starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString((unsigned char)1, (const unsigned char*)"Hello World");
	
	while(1) {
		continue;
	}
}
