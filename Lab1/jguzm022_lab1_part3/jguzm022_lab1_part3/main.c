/*
 * jguzm022_lab1_part3.c
 *
 * Created: 4/3/2019 5:35:11 PM
 * Author : joan1
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

