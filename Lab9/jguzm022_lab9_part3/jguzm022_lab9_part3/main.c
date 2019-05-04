/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 9  Exercise 3 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
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
#define SIZE 43
																                                                                        //                                                                                                                                                                                                                               //                                                                             //
const double notes[SIZE] = {146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 155.56, 155.56, 155.56, 155.56, 155.56, 164.81, 164.81, 164.81, 164.81, 164.81, 174.61, 174.61, 174.61, 174.61, 174.61, 164.81, 164.81, 164.81, 164.81, 164.81, 155.56, 155.56, 155.56, 155.56, 155.56, 261.63};// 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 146.83, 155.56, 155.56, 155.56, 155.56, 155.56, 164.81, 164.81, 164.81, 164.81, 164.81, 174.61, 174.61, 174.61, 174.61, 174.61, 164.81, 164.81, 164.81, 164.81, 164.81, 155.56, 155.56, 155.56, 155.56, 155.56, 261.63 };
const double timeUp[SIZE]   = {7, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1};//,1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 8};
const double timeDown[SIZE] = {7, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 4, 1, 1, 4, 1, 1, 4, 1, 1, 1};// 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 4, 8};
enum states{OFF, PLAY, TIMEDOWN, PRESS}state;                                                                                          //
unsigned char cntr;
unsigned char i;
unsigned char b;
void tick(){
	b = ~PINA & 0x01;
	switch(state){
		case OFF:
			state = b ? PLAY : OFF;
			break;
		
		case PLAY:
			if (cntr <= timeUp[i] && i < SIZE){
				state = PLAY;
			}
			else if (cntr > timeUp[i] && i < SIZE){
				state = TIMEDOWN;
				cntr = 0;
			}
			else if (cntr > timeUp[i] && i >= SIZE && !b){
				state = OFF;
			}
			else {// if (cntr > timeUp[i] && i >= SIZE && b){
				state = PRESS;
			}
			break;
		
		case TIMEDOWN:
			if (cntr <= timeDown[i]){
				state = TIMEDOWN;
			}
			else {
				state = PLAY;
				i++;
				cntr = 0;
			}
			break;
			
		case PRESS:
			state = b ? PRESS : OFF;
			break;
		default:
			state = OFF;
			break;
	}
	
	switch (state){
		case OFF:
			set_PWM(0);
			cntr = 0;
			i = 0;
			break;
		
		case PLAY:
			set_PWM(notes[i]);
			cntr++;
			break;
			
		case TIMEDOWN:
			set_PWM(0);
			cntr++;
			break;
		
		case PRESS:
			break;
			
		default:
			break;
	}
}
int main(void)
{
    /* Replace with your application code */
	//T = 167s
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(34);
	TimerOn();
	PWM_on();
    while (1) 
    {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

