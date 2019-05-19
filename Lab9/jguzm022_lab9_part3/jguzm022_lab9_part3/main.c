/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 9  Exercise 3 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buzzer.c"
#include "timer.c"

#define SIZE 43														                                                                        //                                                                                                                                                                                                                               //                                                                             //
const double notes[SIZE] = {146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 155.56, 155.56, 155.56, 155.56, 155.56, 164.81, 164.81, 164.81, 164.81, 164.81, 174.61, 174.61, 174.61, 174.61, 174.61, 164.81, 164.81, 164.81, 164.81, 164.81, 155.56, 155.56, 155.56, 155.56, 155.56, 261.63};// 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 155.56, 155.56, 155.56, 155.56, 155.56, 164.81, 164.81, 164.81, 164.81, 164.81, 174.61, 174.61, 174.61, 174.61, 174.61, 164.81, 164.81, 164.81, 164.81, 164.81, 155.56, 155.56, 155.56, 155.56, 155.56, 261.63 };
const double timeUp[SIZE]   = {7, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1};//,1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 8};
const double timeDown[SIZE] = {7, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1};// 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 8};
enum states{OFF, PLAY, TIMEDOWN, PRESS}state;                                                                                          //
unsigned char cntr;
unsigned char i;
unsigned char b;
void tick(){
	b = ~PINA & 0x01;
	switch(state){
		case OFF:
			state = b ? PLAY : OFF;
			break;
		
		case PLAY:
			if (cntr <= timeUp[i] && i < SIZE){
				state = PLAY;
			}
			else if (cntr > timeUp[i] && i < SIZE){
				state = TIMEDOWN;
				cntr = 0;
			}
			else if (cntr > timeUp[i] && i >= SIZE && !b){
				state = OFF;
			}
			else {// if (cntr > timeUp[i] && i >= SIZE && b){
				state = PRESS;
			}
			break;
		
		case TIMEDOWN:
			if (cntr <= timeDown[i]){
				state = TIMEDOWN;
			}
			else {
				state = PLAY;
				i++;
				cntr = 0;
			}
			break;
			
		case PRESS:
			state = b ? PRESS : OFF;
			break;
		default:
			state = OFF;
			break;
	}
	
	switch (state){
		case OFF:
			set_PWM(0);
			cntr = 0;
			i = 0;
			break;
		
		case PLAY:
			set_PWM(notes[i]);
			cntr++;
			break;
			
		case TIMEDOWN:
			set_PWM(0);
			cntr++;
			break;
		
		case PRESS:
			break;
			
		default:
			break;
	}
}
int main(void)
{
    /* Replace with your application code */
	//T = 167s
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(34);
	TimerOn();
	PWM_on();
    while (1) 
    {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

