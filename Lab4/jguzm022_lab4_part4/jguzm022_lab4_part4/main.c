/*
 * jguzm022_lab4_part3.c
 *
 * Created: 4/10/2019 11:40:08 AM
 * Author : joan1
 */ 

#include <avr/io.h>

enum DLS_States { DLS_SMStart, LOCK_WAITN, WAITY, UNLOCK_WAITN, WAITY2 } DLS_State;

void TickFct_Latch()
{
    unsigned char tempN  = PINA & 0x04;
     unsigned char tempY  = PINA & 0x02;
	unsigned char tempX  = PINA & 0x01;
	unsigned char tempA7 = PINA & 0x80;

    switch(DLS_State) {   // Transitions
        case DLS_SMStart:  // Initial transition
			DLS_State = LOCK_WAITN;
			break;

        case LOCK_WAITN:
			if (tempN && !tempY && !tempX && !tempA7) {
    			DLS_State = WAITY;
			}
			else if (tempA7 && !tempN && !tempY && !tempX) {
    			DLS_State = LOCK_WAITN;
			}
			else {
    			DLS_State = LOCK_WAITN;
			}
			break;

        case WAITY:
			if (!tempN && tempY && !tempX && !tempA7) {
    			DLS_State = UNLOCK_WAITN;
			}
			else if(!tempN && !tempY && !tempX && !tempA7){
				DLS_State = WAITY;
			}
			else if (tempA7 && !tempN && !tempY && !tempX) {
    			DLS_State = LOCK_WAITN;
			}
			else {
    			DLS_State = LOCK_WAITN;
			}
			break;
        
        case UNLOCK_WAITN:
			if (tempA7 && !tempN && !tempY && !tempX){
		        DLS_State = LOCK_WAITN;
			}
			else if (!tempA7 && tempN && !tempY && !tempX) {
				DLS_State = WAITY2;
			}
			else {
				DLS_State = UNLOCK_WAITN;
			}
			break;
			
		case WAITY2:
			if ((!tempA7 && !tempN && tempY && !tempX) || (tempA7 && !tempN && !tempY && !tempX )) {
				DLS_State = LOCK_WAITN;
			}
			else if (!tempA7 && !tempY && (tempN || tempX)){
				DLS_State = UNLOCK_WAITN;
			}
			else {
				DLS_State = WAITY2;
			}

        default:
			DLS_State = DLS_SMStart;
			break;
    } // Transitions

    switch(DLS_State) {   // State actions
        case DLS_SMStart:
			PORTC = DLS_SMStart;
			break;

        case LOCK_WAITN:
			PORTB = 0x00;
			PORTC = LOCK_WAITN;
			break;
        
        case WAITY:
			PORTC = WAITY;
	        break;
        
        case UNLOCK_WAITN:
			PORTB = 0x01;
			PORTC = UNLOCK_WAITN;
			break;
			
		case  WAITY2:
			PORTC = WAITY2;
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