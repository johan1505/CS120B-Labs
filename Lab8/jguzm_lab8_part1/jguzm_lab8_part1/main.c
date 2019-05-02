/*
 * jguzm_lab8_part1.c
 *
 * Created: 4/28/2019 11:53:10 PM
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
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	ADC_init();
	unsigned short ADCRegister = 0x00;
	unsigned char temp8Bits = 0x00;
	unsigned char temp2Bits = 0x00;
    while (1) 
    {
		ADCRegister = ADC;
		temp8Bits = (char)ADCRegister;
		temp2Bits = (char)(ADCRegister >> 8);
		PORTB = temp8Bits;
		PORTD = temp2Bits;
    }
}

