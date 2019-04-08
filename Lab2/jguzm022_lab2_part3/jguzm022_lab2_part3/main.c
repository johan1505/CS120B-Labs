/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 2  Exercise 3 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{  
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	unsigned char tmpAx = 0x00; // Temporary variable to hold the value of Ax
	unsigned char cntavail;
	while (1)
	{
		cntavail = 0x00;
		for (unsigned char i = 0x01; i <= 0x08 ; i = 2*i){
			tmpAx = PINA & i;
			if (tmpAx == 0x00){
				++ cntavail;
			}
		}
		
		if (cntavail == 0x00){
			cntavail = cntavail | 0x80;
		}
		
		PORTC = cntavail;
	}
	
	return 0;
}

