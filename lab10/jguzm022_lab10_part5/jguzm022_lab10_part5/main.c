/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 10  Exercise 3 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.c"

unsigned char output = 0x00;

enum IC_States {WAIT_B, INC, DEC} IC_state;
unsigned char b;
void tick1(){
	b = ~PINA & 0xFF;
	switch(IC_state){
		case WAIT_B:
			if (!b){
				IC_state = WAIT_B;
			}
			else if (b == 1 && output < 9){
				IC_state = INC;
			}
			else if (b == 2 && output > 0){
				IC_state = DEC;
			}
			else if (b==3){
				IC_state = WAIT_B;
				output = 0;	
			}
			else {
				IC_state = WAIT_B;
			}
			break;
		
		case INC:
			if (b == 1 && output < 9){
				IC_state = INC;
			}
			else {
				IC_state = WAIT_B;
			}
			break;
			
		case DEC:
			if (b == 2 && output > 0){
				IC_state = DEC;
			}
			else {
				IC_state = WAIT_B;
			}
		
		default:
			IC_state = WAIT_B;
			break;
	}
	switch(IC_state){
		case WAIT_B:
			break;
		case INC:
			output++;
			break;
		case DEC:
			output--;
			break;
		default:
			break;
	}
}

enum DLED_States {INIT, DISPLAY}DLED_State;
void tick2(){
	switch(DLED_State){
		case INIT:
			DLED_State = DISPLAY;
			break;
		
		case DISPLAY:
			DLED_State = INIT;
			break;
		
		default:
			DLED_State = INIT;
			break;
	}
	
	switch(DLED_State){
		case INIT:
			break;
		case DISPLAY:
			PORTB = output;	
	}
}


int main(void)
{
    /* Replace with your application code */
	unsigned short IC_elpasedTime = 1000; 
	unsigned short DLED_elapsedTime = 200;
	const unsigned short timerPeriod = 200;
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	IC_state = WAIT_B;
	DLED_elapsedTime = INIT;
	TimerSet(timerPeriod);
	TimerOn();
    while (1) 
    {
		if (IC_elpasedTime >= 1000){
			tick1();
			IC_elpasedTime = 0;
		}
		if (DLED_elapsedTime >= 200){
			tick2();
			DLED_elapsedTime = 0;
		}
		while(!TimerFlag);
		TimerFlag = 0;
		IC_elpasedTime += timerPeriod;
		DLED_elapsedTime += timerPeriod;
    }
}

