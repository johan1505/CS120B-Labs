/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 5  Exercise 3 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#define b PINA & 0x01

unsigned char led = 0x00;
//unsigned char b = 0x00;
enum Lights_States { Lights_SMStart,  START, LIGHTS_LEFT, PRESS1, WAIT1, LIGHTS_RIGHT, PRESS2, WAIT2, PRESS3, WAIT3} Lights_State;
void TickFct_Latch()
{
//	b = PINA & 0x01; // CHANGE
	switch(Lights_State) // transitions
	{
		case  Lights_SMStart:
			Lights_State = START;	
			break;
			
		case START:
			if(!b){
				Lights_State =START;
			}
			else {
				Lights_State = LIGHTS_LEFT;
				led = 0x01;
			}
			break;
		
		case LIGHTS_LEFT:
			if (b){
				Lights_State = PRESS1;
			}
			else {
				Lights_State = WAIT1;
			}
			break;
		
		case PRESS1:
			if(b){
				Lights_State = PRESS1;
			}
			else {
				Lights_State = WAIT1;
			}
			break;		
		
		case WAIT1:
			if (!b){
				Lights_State = WAIT1;
			}
			else if(b && led != 0x7F) {
				Lights_State = LIGHTS_LEFT;
			}
			else {
				Lights_State = LIGHTS_RIGHT;
				led = 0x00;
			}
			break;
		
		case LIGHTS_RIGHT:
			if (b){
				Lights_State = PRESS2;
			}
			else {
				Lights_State = WAIT2; // may cause errors
			}
			break;
		
		case PRESS2:
			if(b){
				Lights_State = PRESS2;
			}
			else {
				Lights_State = WAIT2;
			}
			break;
			
		case WAIT2:
			if (!b){
				Lights_State = WAIT2;
			}
			else if (b && led != 0x3F) {
				Lights_State = LIGHTS_RIGHT;
			}
			else if (b && led == 0x3F) {
				Lights_State = PRESS3;
			}
			break;
			
		case PRESS3:
			if (b){
				Lights_State = PRESS3;
			}
			else {
				Lights_State = WAIT3;
			}
			break;
		
		case WAIT3:
			if (!b){
				Lights_State = WAIT3;
			}
			else {
				Lights_State = LIGHTS_LEFT;
				led = 0x00;
			}
			break;
			
		default:
			Lights_State = Lights_SMStart;
			break;
	}
	
	switch(Lights_State) // State actions
	{
		case START:
			led = 0x00;
			break;
		
		case LIGHTS_LEFT:
			PORTB = led;
			led = (led << 1) + 1;
			break;
			
		case PRESS1:
			break;
		
		case WAIT1:
			break;
			
		case LIGHTS_RIGHT:
			PORTB = led;
			led = (led >> 1) + 32;
			break;
		
		case PRESS2:
			break;
			
		case WAIT2:
			break;
			
		case PRESS3:
			PORTB = led;
			break;
				
		case WAIT3:
			break;
			
		default:
			break;
	}
}

int main(void)
{
	// Declare DDR's and properly initialize them
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	Lights_State = Lights_SMStart;
    while (1) 
    {
		TickFct_Latch();
    }
}

