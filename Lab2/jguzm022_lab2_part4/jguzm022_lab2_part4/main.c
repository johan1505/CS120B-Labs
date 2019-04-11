/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 2  Exercise 4 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include <stdlib.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
    DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
    DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;
    unsigned char tmpD = 0x00;
	unsigned short totalWeight = 0x00;
    
    while (1)
    {
	    tmpD = 0x00; // clear tmpD
	    // read inputs
	    tmpA = PINA;
	    tmpB = PINB;
		tmpC = PINC;
		
		totalWeight = tmpA + tmpB + tmpC; // Use it to approximate the weight.
	    
	    if ( totalWeight > 0x8C ){ // If total weight > 140 - cart exceeds max weight
		    tmpD = tmpD | 0x01;
	    }
	    if ( abs(tmpA - tmpC) > 0x50 ){		// If  |Weight A - Weight C| > 80 -  cart is not balanced
		    tmpD = tmpD | 0x02;
	    }
		
		//PINC RANDOMLY CHANGES??
		//Testing Github 
		if (totalWeight >= 0 && totalWeight < 255){
			tmpD = tmpD | 0x04; // set the bit 2 to 1; 
		}
		else if (totalWeight > 255 && totalWeight < 510){
			tmpD = tmpD | 0x08; // set the bit 4 to 1; 
		}
		else {
			tmpD = tmpD | 0x10; // set the bit 5 to 1;
		}
		
	    PORTD = tmpD;
    }
}

