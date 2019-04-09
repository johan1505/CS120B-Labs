/*
 * jguzm022_lab3_part1.c
 *
 * Created: 4/8/2019 8:43:49 AM
 * Author : joan1
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

//Count the number of 1s on ports A and B and output that number on port C
int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // configure port C's 8 pins as outputs
	
	unsigned char counter = 0x00;
	unsigned char A  = PINA;
	unsigned char B  = PINB;
    while (1) 
    {
		A = PINA;
		B = PINB;
		counter = 0x00;
		for( unsigned char i = 0x00; i < 0x08 ; ++i ){
			if(GetBit(A, i)){
				++counter;
			}
			if(GetBit(B,i)){
				++counter;
			}		
		}
		PORTC = counter;
    }
}

