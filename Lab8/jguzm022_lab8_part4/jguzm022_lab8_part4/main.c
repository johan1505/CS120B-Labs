/*
 * jguzm022_lab8_part4.c
 *
 * Created: 4/29/2019 9:02:42 PM
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
	unsigned short ADCRegister = 0x00;
	while (1)
	{
		ADCRegister = ADC;
		if (ADCRegister <= 128){
			PORTB = 0x01;
		}
		else if (ADCRegister <= 256)  {
			PORTB = 0x03;
		}
		else if (ADCRegister <= 383)  {
			PORTB = 0x07;
		}
		else if (ADCRegister <= 511)  {
			PORTB = 0x0F;
		}
		else if (ADCRegister <= 639)  {
			PORTB = 0x1F;
		}
		else if (ADCRegister <= 767)  {
			PORTB = 0x3F;
		}
		else if (ADCRegister <= 895)  {
			PORTB = 0x07F;
		}
		else if (ADCRegister <= 1023) {
			PORTB = 0xFF;
		}
	}
}

