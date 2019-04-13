/*
 * jguzm022_lab4_part1.c
 *
 * Created: 4/10/2019 8:34:53 AM
 * Author : joan1
 */ 

#include <avr/io.h>

enum LED_States { LED_SMStart, LED0_ON, HOLD, LED1_ON } LED_State;
unsigned char Checker = 0;
void TickFct_Latch()
{
	unsigned char tempA0 = PINA & 0x01;
	switch(LED_State) {   // Transitions
		case LED_SMStart:  // Initial transition
			LED_State = LED0_ON;
			break;

		case LED0_ON:
			if (tempA0) {
				LED_State = LED0_ON;
			}
			else if (!tempA0) {
				LED_State = HOLD;
			}
			break;

		case HOLD:
			if(!tempA0){
				LED_State =	HOLD;
			}
			else if (tempA0 && !Checker){
				LED_State = LED1_ON;
			}
			else if (tempA0 && Checker){
				LED_State = LED0_ON;
			}
			break;
		case LED1_ON:
			if (tempA0) {
				LED_State = LED1_ON;
			}
			else if (!tempA0) {
				LED_State = HOLD;
			}
			break;

		default:
			LED_State = LED_SMStart;
			break;
	} // Transitions

	switch(LED_State) {   // State actions
		case LED_SMStart:
			break;
		
		case LED0_ON:
			PORTB = 0x01;
			Checker = 0;
			break;
			
		case HOLD:
			break;
			
		case LED1_ON:
			Checker = 1;
			PORTB = 0x02;
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
	
	LED_State = LED_SMStart;
	
    while (1) 
    {
		TickFct_Latch();
    }
}

