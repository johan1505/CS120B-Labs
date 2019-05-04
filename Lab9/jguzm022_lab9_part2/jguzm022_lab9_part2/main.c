/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 9  Exercise 2 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
const double C4 = 261.63;
const double D4 = 293.66;
const double E4 = 329.63;
const double F4 = 349.23;
const double G4 = 392.00;
const double A4 = 440.00;
const double B4 = 493.88;
const double C5 = 523.25;
unsigned char tracker;
unsigned char b0 = 0, b1 = 0,b2 = 0;
enum states { stateStart, WAIT, PRESSA0, OUTPUT_SOUND, PRESSA1, PRESSA2} state;
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

void tick(){
	b0 = ~PINA & 0x01;
	b1 = ~PINA & 0x02;
	b2 = ~PINA & 0x04;
	switch (state){
		case stateStart:
			state = WAIT;
			tracker = 0;
			set_PWM(0);
			break;
			
		case WAIT:
			if (!b0 && !b1 && !b2){
				state = WAIT;
			}
			else if (b0 && !b1 && !b2){
				state = PRESSA0;
				tracker = 0;
				set_PWM(C4);
			}
			else {
				state = WAIT;
			}
			break;
			
		case PRESSA0:
			if (b0){
				state = PRESSA0;
			}
		    else {
				state = OUTPUT_SOUND; 
			}
			
		case OUTPUT_SOUND:
			if (!b0 && !b1 && !b2){
				state = OUTPUT_SOUND;
			}
			else if (b0 && !b1 && !b2){
				state = WAIT;
			    set_PWM(0);

			}
			else if (!b0 && b1 && !b2){
				state = PRESSA1;
				tracker = tracker != 7 ? ++tracker : tracker;
				if (tracker == 0){
					set_PWM(C4);	
				}
				else if (tracker == 1){
					set_PWM(D4);
				}
				else if (tracker == 2){
					set_PWM(E4);
				}
				else if (tracker == 3){
					set_PWM(F4);	
				}
				else if (tracker == 4){
					set_PWM(G4);
				}
				else if (tracker == 5){
					set_PWM(A4);
				}
				else if (tracker == 6){
					set_PWM(B4);
				}
				else {
					set_PWM(C5);
				}
			}
			else if (!b0 && !b1 && b2){
				state = PRESSA2;
				tracker = tracker != 0 ? --tracker : tracker;
				if (tracker == 0){
					set_PWM(C4);
				}
				else if (tracker == 1){
					set_PWM(D4);
				}
				else if (tracker == 2){
					set_PWM(E4);
				}
				else if (tracker == 3){
					set_PWM(F4);
				}
				else if (tracker == 4){
					set_PWM(G4);
				}
				else if (tracker == 5){
					set_PWM(A4);
				}
				else if (tracker == 6){
					set_PWM(B4);
				}
				else {
					set_PWM(C5);
				}
			}
			break;
		
		case PRESSA1:
			if (b1){
				state = PRESSA1;
			}
			else {
				state = OUTPUT_SOUND;
			}
			break;
			
		case PRESSA2:
			if (b2){
				state = PRESSA2;
			}
			else {
				state = OUTPUT_SOUND;
			}
			break;
		default:
			state =	WAIT;
			break;
	}
	
	switch (state){
		case stateStart:
			break;
		case WAIT:
			break;
		case PRESSA1:
			break;
		case PRESSA2:
			break;
		default:
			break;
	}
}
int main(void)
{;
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(250);
	TimerOn();
	PWM_on();
    while (1) 
    {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }

}

