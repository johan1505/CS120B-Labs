/*
 * jguzm022_lab9_part1.c
 *
 * Created: 5/1/2019 7:58:45 AM
 * Author : joan1
 */ 

#include <avr/io.h>

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0 | 1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


enum states {WAIT, PRESSA0, PRESSA1, PRESSA2}state;
unsigned char button = 0x00;

void tick(){
	button = ~PINA & 0x07;
	switch(state){
		case WAIT:
			if (button == 1){
				state = PRESSA0;
			}
			else if (button == 2){
				state = PRESSA1;
			}
			else if (button == 4){
				state = PRESSA2;
			}
			else {
				state = WAIT;
			}
			break;
			
		case PRESSA0:
			if (button == 1){
				state = PRESSA0;
			}
			else {
				state =	WAIT;
			}
			break;
		
		case PRESSA1:
			if (button == 2){
				state = PRESSA1;
			}
			else {
				state =	WAIT;
			}
			break;
			
		case PRESSA2:
			if (button == 4){
				state = PRESSA2;
			}
			else {
				state =	WAIT;
			}
			break;
		
		default:
			state =	WAIT;
			break;
	}
	
	switch (state){
		case WAIT:
			set_PWM(0);
			break;
		case PRESSA0:
			set_PWM(261.63);
			break;
		case PRESSA1:
			set_PWM(293.66);
			break;
		case PRESSA2:
			set_PWM(329.63);
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
	PWM_on();
	state = WAIT;
    while (1) 
    {
		tick();
    }
}

