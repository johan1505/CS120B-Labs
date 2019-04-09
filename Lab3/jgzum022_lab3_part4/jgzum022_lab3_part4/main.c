/*
 * jgzum022_lab3_part4.c
 *
 * Created: 4/8/2019 5:45:58 PM
 * Author : joan1
 */ 

#include <avr/io.h>


int main(void)
{
    /* (Challenge): Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4. That is to say,  take the upper nibble of PINA and map it to the lower nibble of PORTB,
	 likewise take the lower nibble of PINA and map it to the upper nibble of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).
 */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x0F; PORTB = 0xF0;
	DDRC = 0xF0; PORTC = 0x0F;
	unsigned char inputA = 0x00;
	unsigned char outputB = 0x00;
	unsigned char outputC = 0x00; 
    while (1) 
    {
		inputA = PINA & 0xFF;
		outputB = inputA >> 4;
		outputC = inputA << 4;
		PORTB = outputB;
		PORTC = outputC;
    }
}

