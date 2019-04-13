/*
 * jguzm022_lab4_part2.c
 *
 * Created: 4/10/2019 9:08:33 AM
 * Author : joan1
 */ 

#include <avr/io.h>

enum IDC_States { IDC_SMStart, INIT, INC, PRESS_INC,  DEC, PRESS_DEC, WAIT, RESET  } IDC_State;

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
				IDC_State = INC;
			}
			else if (tempA1 && !tempA0) {
				IDC_State = DEC;
			}
			else if (tempA0 && tempA1){
				IDC_State = RESET;
			}
			else {
				IDC_State = INIT;
			}
			break;
				
		case INC:
			if (tempA0){
				IDC_State = PRESS_INC;
			}
			else if (!tempA0){
				IDC_State = WAIT;
			}
			break;
			
		case PRESS_INC:
			if (tempA0 && !tempA1){
				IDC_State = PRESS_INC;
			}
			else if (tempA0 && tempA1){
				IDC_State = RESET;
			}
			else if (!tempA0){
				IDC_State = WAIT;
			}
			break;
			
		case DEC:
			if (tempA1){
				IDC_State = PRESS_DEC;
			}
			else if (!tempA1){
				IDC_State = WAIT;
			}
			break;
			
		case PRESS_DEC:
			if (tempA1 && !tempA0){
				IDC_State = PRESS_DEC;
			}
			else if (tempA0 && tempA1){
				IDC_State = RESET;
			}
			else if (!tempA1){
				IDC_State = WAIT;
			}
			break;
			
		case WAIT:
			if (tempA0 && !tempA1 && (PORTC < 9)){
				IDC_State = INC;	
			}
			else if (tempA1 && !tempA0 && (PORTC > 0)){
				IDC_State = DEC;	
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

		case INC:
			PORTC = PORTC + 1;
			break;
			
		case PRESS_INC:
			break;
			
		case DEC:
			PORTC = PORTC - 1;
			break;
		
		case PRESS_DEC:
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

