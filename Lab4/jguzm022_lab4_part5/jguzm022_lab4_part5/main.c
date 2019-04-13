/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 4  Exercise 5 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum DLS_States { DLS_SMStart, Lock_wait1, Press1, Wait2, Press2, Wait3, Press3, Wait4, Compare, Unlock } DLS_State;
unsigned char correctCode[] = {0x04, 0x01, 0x02, 0x01};
unsigned char userInput[] = {0x00, 0x00, 0x00, 0x00};

void TickFct_Latch()
{
    unsigned char tempN  = PINA & 0x04;
    unsigned char tempY  = PINA & 0x02;
	unsigned char tempX  = PINA & 0x01;
	unsigned char tempA7 = PINA & 0x80;

    switch(DLS_State) {   // Transitions
        case DLS_SMStart:  // Initial transition
			DLS_State = Lock_wait1;
			break;

        case Lock_wait1:
			if ((tempX || tempY || tempN) && !tempA7) {
    			DLS_State = Press1;
				userInput[0] = (userInput[0] & 0xFB) | tempN;
			}
			else if (tempA7 && !tempN && !tempY && !tempX) {
    			DLS_State = Lock_wait1;
			}
			else if(!(tempA7 || tempN || tempY || tempX)) {
				DLS_State = Lock_wait1;
			}
			break;

        case Press1:
			if ((tempN || tempY || tempX) && !tempA7) {
    			DLS_State = Press1;
			}
			else if (!(tempN || tempY || tempX || tempA7)) {
    			DLS_State = Wait2;
			}
			break;
        
        case Wait2:
			if ((tempX || tempY || tempN) && !tempA7) {
				DLS_State = Press2;
				userInput[1] = (userInput[1] & 0xFE) | tempX;
			}
			else if (tempA7 && !tempN && !tempY && !tempX) {
				DLS_State = Lock_wait1;
			}
			else if(!(tempA7 || tempN || tempY || tempX)) {
				DLS_State = Wait2;
			}
			
			break;
			
		case Press2:
			if ((tempN || tempY || tempX) && !tempA7) {
				DLS_State = Press2;
			}
			else if (!(tempN || tempY || tempX || tempA7)) {
				DLS_State = Wait3;
			}
			
			break;
		
		case Wait3:
			if ((tempX || tempY || tempN) && !tempA7) {
				DLS_State = Press3;
				userInput[2] = (userInput[2] & 0xFD) | tempY;
			}
			else if (tempA7 && !tempN && !tempY && !tempX) {
				DLS_State = Lock_wait1;
			}
			else if(!(tempA7 || tempN || tempY || tempX)) {
				DLS_State = Wait3;
			}
			
			break;
			
		case Press3:
			if ((tempN || tempY || tempX) && !tempA7) {
				DLS_State = Press3;
			}
			else if (!(tempN || tempY || tempX || tempA7)) {
				DLS_State = Wait4;
			}
			
			break;
			
		case Wait4:
			if ((tempX || tempY || tempN) && !tempA7) {
				DLS_State = Compare;
				userInput[3] = (userInput[3] & 0xFE) | tempX;
			}
			else if (tempA7 && !tempN && !tempY && !tempX) {
				DLS_State = Lock_wait1;
			}
			else if(!(tempA7 || tempN || tempY || tempX)) {
				DLS_State = Wait4;
			}
	
			break;
		
		case Compare:
			if (userInput[0] == correctCode[0] && userInput[1] == correctCode[1] && userInput[2] == correctCode[2] && userInput[3] == correctCode[3]){
				DLS_State = Unlock;
			}
			else {
				DLS_State = Lock_wait1;
			}
			
			break;
		
		case Unlock:
			if(tempA7 && !(tempN || tempY || tempX)){
				DLS_State = Lock_wait1;	
			}
			else {
				DLS_State = Unlock;
			}
			
			break;
			
        default:
			DLS_State = DLS_SMStart;
			break;
			
	} // Transitions

    switch(DLS_State) {   // State actions
        case DLS_SMStart:
			PORTC = DLS_SMStart;
			break;

        case Lock_wait1:
			PORTB = 0x00;
			PORTC = Lock_wait1;
			break;
        
        case Press1:
			PORTC = Press1;
	        break;
        
        case Wait2:
			PORTC = Wait2;
			break;
			
		case  Press2:
			PORTC = Press2;
			break;
			
		case Wait3:
			PORTC = Wait3;
			break;
		
		case Press3:
			PORTC = Press3;
			break;
			
		case Wait4:
			PORTC = Wait4;
			break;
		
		case  Compare:
			PORTC = Compare;
			break;
		
		case Unlock:
			PORTB = 0x01;
			PORTC = Unlock;
			break;
			
        default:
			break;
	} // State actions
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
    DLS_State = DLS_SMStart;
    while (1)
    {
        TickFct_Latch();
    }
}