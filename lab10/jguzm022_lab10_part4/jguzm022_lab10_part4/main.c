/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 10  Exercise 3 
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

unsigned char b = 0x00;
unsigned char noise = 0x00;
unsigned char threeLEDS = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char limit = 2;


enum tLEDStates {START1, ON1, ON2, ON3}tLEDState;
void tick1(){
	switch(tLEDState){
		case START1:
			tLEDState = ON1;
			break;
		
		case ON1:
			tLEDState = ON2;
			break;
		
		case ON2:
			tLEDState = ON3;
			break;
		
		case ON3:
			tLEDState = ON1;
			break;
		
		default:
			tLEDState = START1;
			break;
	}
	
	switch(tLEDState){
		case START1:
			break;
		
		case ON1:
			threeLEDS = 0x01;
			break;
		
		case ON2:
			threeLEDS = 0x02;
			break;
		
		case ON3:
			threeLEDS = 0x04;
			break;
		
		default:
			break;
	}
}

enum bLEDStates {START2, ON, OFF}bLEDState;
void tick2(){
	switch(bLEDState){
		case START2:
			bLEDState = ON;
			break;
		
		case ON:
			bLEDState = OFF;
			break;
		
		case OFF:
			bLEDState = ON;
			break;
		
		default:
			break;
	}
	
	switch(bLEDState){
		case START2:
			break;
		
		case ON1:
			blinkingLED = 0x08;
			break;
		
		case OFF:
			blinkingLED = 0x00;
			break;
		
		default:
			break;
	}
}

enum s_States {START3, SOUND_ON, SOUND_OFF}s_State;
unsigned char i = 0x00;
void tick3(){
	b = ~PINA & 0x04;
	switch(s_State){
		case START3:
			///noise = 0x00;
			if (!b){
				s_State = START3;
			}
			else {
				s_State = SOUND_ON;
				i = 0;
			}
			break;
			
		case SOUND_ON:
			if (b && i < limit){
				s_State = SOUND_ON;
			}
			else if (b && i >= limit){
				s_State = SOUND_OFF;
				i = 0;				
			}
			else {
				s_State = START3;
			}
			break;
	
		case SOUND_OFF:
			if (b && i < limit){
				s_State = SOUND_OFF;
			}
			else if (b && i >= limit){
				s_State = SOUND_ON;
				i = 0;
			}
			else {
				s_State = START3;
			}
			break;
			
		default:
			s_State = START3;
			break;
	}
	
	switch(s_State){
		case START3:
			noise = 0x00;
			break;
		
		case SOUND_ON:
			noise = 0x10;
			i++;
			break;
			
		case SOUND_OFF:
			noise = 0x00;
			i++;
			break;	
		
		default:
			break;
	}
}

enum gb_States {START4, PRESS_UP, PRESS_DOWN} gb_State;
void tick4(){
	b = ~PINA & 0xFF;	
	switch(gb_State){
		case START4:
			if(!b){
				gb_State = START4;
			}
			else if (b == 1){
				gb_State = PRESS_UP;
				++limit;
			}
			else if (b == 2 && limit > 1){
				gb_State = PRESS_DOWN;
				--limit;
			}
			else {
				gb_State = START4;
			}
		case PRESS_UP:
			gb_State = b == 1 ? PRESS_UP : START4;
			break;
		
		case PRESS_DOWN:
			gb_State = b == 2 ? PRESS_DOWN : START4;
			break;
		
		default:
			gb_State = START4;
			break;
	}
	
	switch(gb_State){
		case START4:
			break;
		case PRESS_UP:
			break;
		case PRESS_DOWN:
			break;
		default:
			break;
	}
}

//enum cLEDStates {START5, OUTPUT} cLEDState;
void combTick(){
	PORTB = (PINB & 0x00) | blinkingLED | threeLEDS | noise;
}

int main(void)
{
    /* Replace with your application code */
	unsigned short t_LED_elapsedTime = 300; 
	unsigned short b_LED_elapsedTime = 1000;
	unsigned short s_elapsedTime = 1;
	unsigned short gb_elapsedTime = 100;
	const unsigned char timerPeriod = 1;
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	tLEDState = START1;
	bLEDState = START2;
	s_State = START3;
	gb_State = START4;
	TimerSet(timerPeriod);
	TimerOn();
    while (1) 
    {
		if (t_LED_elapsedTime >= 300){
			tick1();
			t_LED_elapsedTime = 0;
		}
		if (b_LED_elapsedTime >= 1000){
			tick2();
			b_LED_elapsedTime = 0;
		}
		if (s_elapsedTime >= 1){
			tick3();
			s_elapsedTime = 0;
		}
		if (gb_elapsedTime >= 100){
			tick4();
			gb_elapsedTime = 0;
		}
		combTick();
		while(!TimerFlag);
		TimerFlag = 0;
		t_LED_elapsedTime += timerPeriod;
		b_LED_elapsedTime += timerPeriod;
		s_elapsedTime += timerPeriod;
		gb_elapsedTime += timerPeriod;
    }
}

