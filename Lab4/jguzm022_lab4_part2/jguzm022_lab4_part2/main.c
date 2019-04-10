/*
 * jguzm022_lab4_part2.c
 *
 * Created: 4/10/2019 9:08:33 AM
 * Author : joan1
 */ 

#include <avr/io.h>

enum IDC_States { IDC_SMStart, INIT, INCREMENT, DECREMENT, WAIT, RESET  } IDC_State;

void TickFct_Latch()
{
	unsigned char tempA0 = PINA & 0x01;
	unsigned char tempA1 = PINA & 0x02;
	switch(IDC_State) {   // Transitions
		case IDC_SMStart:  // Initial transition
			IDC_State = INIT;
			break;

		case INIT:
			if (tempA0 && !tempA1) {
				IDC_State = INCREMENT;
			}
			else if (tempA1 && !tempA0) {
				IDC_State = DECREMENT;
			}
			else if (tempA0 && tempA1){
				IDC_State = RESET;
			}
			else {
				IDC_State = INIT;
			}
			break;

		case INCREMENT:
			IDC_State = WAIT;
			break;
			
		case DECREMENT:
			IDC_State = WAIT;
			break;
			
		case WAIT:
			if (tempA0 && !tempA1 && (PORTC < 9)){
				IDC_State = INCREMENT;	
			}
			else if (tempA1 && !tempA0 && (PORTC > 0)){
				IDC_State = DECREMENT;	
			}
			else if (tempA0 && tempA1){
				IDC_State = RESET;
			}
			else if ((tempA0 && (PINC >= 9)) || (!tempA0 && !tempA1) || (tempA1 && (PORTC == 0))){
				IDC_State = WAIT;
			}
			break;
			
		case RESET:
			IDC_State = WAIT;
			break;
			
		default:
			IDC_State = IDC_SMStart;
			break;
	} // Transitions

	switch(IDC_State) {   // State actions
		case INIT:
			PORTC = 0x07;
			break;

		case INCREMENT:
			PORTC = PORTC + 1;
			break;
		
		case DECREMENT:
			PORTC = PORTC - 1;
			break;
		
		case WAIT:
			break;
			
		case RESET:
			PORTC = 0x00;
			break;
			
		default:
		break;
	} // State actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	IDC_State = IDC_SMStart;
    while (1) 
    {
		TickFct_Latch();
    }
}

