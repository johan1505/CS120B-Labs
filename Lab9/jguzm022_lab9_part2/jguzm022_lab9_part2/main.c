/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 9  Exercise 2 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buzzer.c"
#include "timer.c"

const double C4 = 261.63;
const double D4 = 293.66;
const double E4 = 329.63;
const double F4 = 349.23;
const double G4 = 392.00;
const double A4 = 440.00;
const double B4 = 493.88;
const double C5 = 523.25;
unsigned char tracker;
unsigned char b0 = 0, b1 = 0,b2 = 0;
enum states { stateStart, WAIT, PRESSA0, OUTPUT_SOUND, PRESSA1, PRESSA2} state;
	
void tick(){
	b0 = ~PINA & 0x01;
	b1 = ~PINA & 0x02;
	b2 = ~PINA & 0x04;
	switch (state){
		case stateStart:
			state = WAIT;
			tracker = 0;
			set_PWM(0);
			break;
			
		case WAIT:
			if (!b0 && !b1 && !b2){
				state = WAIT;
			}
			else if (b0 && !b1 && !b2){
				state = PRESSA0;
				tracker = 0;
				set_PWM(C4);
			}
			else {
				state = WAIT;
			}
			break;
			
		case PRESSA0:
			if (b0){
				state = PRESSA0;
			}
		    else {
				state = OUTPUT_SOUND; 
			}
			
		case OUTPUT_SOUND:
			if (!b0 && !b1 && !b2){
				state = OUTPUT_SOUND;
			}
			else if (b0 && !b1 && !b2){
				state = WAIT;
			    set_PWM(0);

			}
			else if (!b0 && b1 && !b2){
				state = PRESSA1;
				tracker = tracker != 7 ? ++tracker : tracker;
				if (tracker == 0){
					set_PWM(C4);	
				}
				else if (tracker == 1){
					set_PWM(D4);
				}
				else if (tracker == 2){
					set_PWM(E4);
				}
				else if (tracker == 3){
					set_PWM(F4);	
				}
				else if (tracker == 4){
					set_PWM(G4);
				}
				else if (tracker == 5){
					set_PWM(A4);
				}
				else if (tracker == 6){
					set_PWM(B4);
				}
				else {
					set_PWM(C5);
				}
			}
			else if (!b0 && !b1 && b2){
				state = PRESSA2;
				tracker = tracker != 0 ? --tracker : tracker;
				if (tracker == 0){
					set_PWM(C4);
				}
				else if (tracker == 1){
					set_PWM(D4);
				}
				else if (tracker == 2){
					set_PWM(E4);
				}
				else if (tracker == 3){
					set_PWM(F4);
				}
				else if (tracker == 4){
					set_PWM(G4);
				}
				else if (tracker == 5){
					set_PWM(A4);
				}
				else if (tracker == 6){
					set_PWM(B4);
				}
				else {
					set_PWM(C5);
				}
			}
			break;
		
		case PRESSA1:
			if (b1){
				state = PRESSA1;
			}
			else {
				state = OUTPUT_SOUND;
			}
			break;
			
		case PRESSA2:
			if (b2){
				state = PRESSA2;
			}
			else {
				state = OUTPUT_SOUND;
			}
			break;
		default:
			state =	WAIT;
			break;
	}
	
	switch (state){
		case stateStart:
			break;
		case WAIT:
			break;
		case PRESSA1:
			break;
		case PRESSA2:
			break;
		default:
			break;
	}
}
int main(void)
{;
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(250);
	TimerOn();
	PWM_on();
    while (1) 
    {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }

}

