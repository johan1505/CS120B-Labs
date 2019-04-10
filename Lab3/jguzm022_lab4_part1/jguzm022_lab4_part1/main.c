/*
 * jguzm022_lab4_part1.c
 *
 * Created: 4/10/2019 8:34:53 AM
 * Author : joan1
 */ 

#include <avr/io.h>

enum LED_States { LED_SMStart, LED0_ON, LED1_ON } LED_State;

void TickFct_Latch()
{
	switch(LED_State) {   // Transitions
		case LED_SMStart:  // Initial transition
			LED_State = LED0_ON;
			break;

		case LED0_ON:
			if ((PINA & 0x01)) {
				LED_State = LED1_ON;
			}
			else if (!(PINA & 0x01)) {
				LED_State = LED0_ON;
			}
			break;

		case LED1_ON:
			if ((PINA & 0x01)) {
				LED_State = LED0_ON;
			}
			else if (!(PINA & 0x01)) {
				LED_State = LED1_ON;
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
			break;
			
		case LED1_ON:
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

