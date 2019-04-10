/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 3  Exercise 2 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char fuelSensor = 0x00;
	unsigned char fuelLed    = 0x00;
    while (1) 
    {
 		PORTC = 0x00;
		fuelSensor = PINA & 0x0F;
		fuelLed    = 0x00;
		if(fuelSensor == 0){
			fuelLed = 0x40;	
		}
		else if (fuelSensor == 1 || fuelSensor == 2){
			fuelLed = (fuelLed | 0x20) | 0x40;
		}
		else if(fuelSensor == 3 || fuelSensor == 4){
			fuelLed = (fuelLed | 0x30) | 0x40;
		}
		else if(fuelSensor == 5 || fuelSensor == 6){
			fuelLed = fuelLed | 0x38;
		}
		else if(fuelSensor == 7 || fuelSensor == 8 || fuelSensor == 9){
			fuelLed = fuelLed | 0x3C;
		}
		else if(fuelSensor == 10 || fuelSensor == 11 || fuelSensor == 12){
			fuelLed = fuelLed | 0x3E;
		}
		else if(fuelSensor == 13 || fuelSensor == 14 || fuelSensor == 15){
			fuelLed = fuelLed | 0x3F;
		}
		
		PORTC = fuelLed;
    }
}

