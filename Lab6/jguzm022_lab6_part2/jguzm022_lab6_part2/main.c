/*
 * jguzm022_lab4_part1.c
 *
 * Created: 4/22/2019 12:59:15 PM
 * Author : joan1
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

enum LED_States {LED_Start, START, ON1, ON2, ON3, PRESSB, WAIT} LED_State;
enum directions {LEFT, RIGHT} direc;
unsigned char b = 0x00;
void tick(){
	b = ~PINA & 0x01;
	switch (LED_State){
		case LED_Start:
			LED_State = START;
			break;
		case START:
			LED_State = ON1;
			break;
		case ON1:
			if (b){
				LED_State = PRESSB;
			}
			else {
				LED_State = ON2;
			}
			break;
		case ON2:
			if (b){
				LED_State = PRESSB;
			}
			else if (direc == LEFT && !b) {
				LED_State = ON3;
			}
			else if (direc == RIGHT && !b){
				LED_State = ON1;	
			}
			break;
		case ON3:
			if (b){
				LED_State = PRESSB;
			}
			else {
				LED_State = ON2;
			}
			break;
		case PRESSB:
			if (b){
				LED_State = PRESSB;
			}
			else {
				LED_State = WAIT;
			}
			break;
		case WAIT:
			if (!b) {
				LED_State = WAIT;
			}
			else if (PORTB == 0x02 && direc == RIGHT && b){
				LED_State = ON1;
			}
			else if (PORTB == 0x02 && direc == LEFT && b){
				LED_State = ON3;
			}
			else {
				LED_State = ON1; // errors??
			}
			break;
	}
	
	switch (LED_State){
		case LED_Start:
			break;
		case START:
			PORTB = 0x00;
			direc = LEFT;
			break;
		case ON1:
			PORTB = 0x01;
			direc = LEFT;
			break;
		case ON2:
			PORTB = 0x02;
			break;
		case ON3:
			PORTB = 0x04;
			direc = RIGHT;
			break;
		case PRESSB:
			break;
		case WAIT: 
			break;
		default:
			break;
	}
}

int main()
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
    LED_State = LED_Start;
	TimerSet(300);
	TimerOn();
    while (1) 
    {
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}


