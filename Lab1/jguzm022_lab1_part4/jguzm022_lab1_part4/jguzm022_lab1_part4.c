/*
 * jguzm022_lab1_part4.c
 *
 * Created: 4/4/2019 3:10:46 PM
 * Author : Johan
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
	unsigned char totalWeight = 0x00;
    
    while (1)
    {
	    tmpD = 0x00; // clear tmpD
	    // read inputs
	    tmpA = PINA;
	    tmpB = PINB;
		tmpC = PINC;
		
		totalWeight = tmpA + tmpB + tmpC; // Use it to approximate the weight.
	    
	    if ( (tmpA + tmpB + tmpC) > 0x8C ){ // If total weight > 140 - cart exceeds max weight
		    tmpD = tmpD | 0x01;
	    }
	    if ( abs(tmpA - tmpC) > 0x50 ){		// If  |Weight A - Weight C| > 80 -  cart is not balanced
		    tmpD = tmpD | 0x02;
	    }
		
		//PINC RANDOMLY CHANGES??
		//Testing Github 
		totalWeight = totalWeight << 2;
	    PORTD = totalWeight | tmpD;
    }
}

