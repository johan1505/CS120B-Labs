/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 3  Exercise 5
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    /* (Challenge): A car's passenger-seat weight sensor outputs a 9-bit value (ranging from 0 to 511) and connects to input PD7..PD0PB0 on the micro=controller. 
	If the weight is equal to or above 70 pounds, the airbag should be enabled by setting PB1 to 1. 
	If the weight is above 5 but below 70, the airbag should be disabled and an "Airbag disabled" icon should light by setting PB2 to 1.
	 (Neither B1 nor B2 should be set if the weight is 5 or less, as there is no passenger).
 */
	DDRB = 0x06; PORTB = 0xF9;
	DDRD = 0x00; PORTD = 0xFF;
	
	unsigned short temp1 = 0x00;
	unsigned short temp2 = 0x00;
	unsigned short seatWeightSensor = 0x00;
	unsigned char  airbag;
    while (1) 
    {
		airbag = 0x00;
		temp1  = PIND << 1; 
		temp2  = PINB & 0x01;
		seatWeightSensor = temp1 + temp2;
		
		if (seatWeightSensor > 5 && seatWeightSensor < 70){
			airbag = 0x04;
		}
		else if (seatWeightSensor >= 70) {
			airbag = 0x02;
		}
		
		PORTB = airbag;
    }
}

