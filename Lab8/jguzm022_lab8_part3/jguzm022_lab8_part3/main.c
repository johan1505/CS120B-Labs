/*
 * jguzm022_lab8_part3.c
 *
 * Created: 4/29/2019 9:08:19 AM
 * Author : joan1
 */ 

#include <avr/io.h>


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	ADC_init();
	const unsigned short MAX = 1023;
	unsigned short ADCRegister = 0x00;
    while (1) 
    {
		ADCRegister = ADC;
		if (ADCRegister >= (MAX/2)){
			PORTB = 0x01;	
		}
		else  {
			PORTB = 0x00;	
		}
    }
}

