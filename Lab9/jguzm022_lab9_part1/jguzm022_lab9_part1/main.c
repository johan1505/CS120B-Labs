/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 9  Exercise 1 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "buzzer.c"

enum states {WAIT, PRESSA0, PRESSA1, PRESSA2} state;
unsigned char button = 0x00;

void tick(){
	button = ~PINA & 0x07;
	switch(state){
		case WAIT:
			if (button == 1){
				state = PRESSA0;
			}
			else if (button == 2){
				state = PRESSA1;
			}
			else if (button == 4){
				state = PRESSA2;
			}
			else {
				state = WAIT;
			}
			break;
			
		case PRESSA0:
			if (button == 1){
				state = PRESSA0;
			}
			else {
				state =	WAIT;
			}
			break;
		
		case PRESSA1:
			if (button == 2){
				state = PRESSA1;
			}
			else {
				state =	WAIT;
			}
			break;
			
		case PRESSA2:
			if (button == 4){
				state = PRESSA2;
			}
			else {
				state =	WAIT;
			}
			break;
		
		default:
			state =	WAIT;
			break;
	}
	
	switch (state){
		case WAIT:
			set_PWM(0);
			break;
		case PRESSA0:
			set_PWM(261.63);
			break;
		case PRESSA1:
			set_PWM(293.66);
			break;
		case PRESSA2:
			set_PWM(329.63);
			break;
		default:
			break;
	}
}
int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	PWM_on();
	state = WAIT;
    while (1) 
    {
		tick();
    }
}

