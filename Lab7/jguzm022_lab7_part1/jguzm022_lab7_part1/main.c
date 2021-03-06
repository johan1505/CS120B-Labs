/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 7  Exercise 1 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include "io.c"
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


enum ScreenStates {STATE_Start, INIT, INC, DEC, WAIT} ScreenState;
unsigned char b1 = 0x00;
unsigned char b0 = 0x00;
unsigned char number = 0;

void tick(){
	b0 = ~PINA & 0x01;
	b1 = ~PINA & 0x02;
	switch(ScreenState){
		case STATE_Start:
			ScreenState = INIT;
			number = 0;
			break;
		
		case INIT:
			if ((!b0 && !b1) || (b0 && b1) || (b1)) {
				ScreenState = INIT;
			}
			else {
				ScreenState = INC;
			}
			break;
			
		case INC:
			if (b0 && b1) {
				ScreenState = INIT;
			}
			else if (b0 && !b1 && number < 9) {
				ScreenState = INC;	
			}
			else{
				ScreenState = WAIT;
			}
			break;
			
		case DEC:
			if (b0 && b1) {
				ScreenState = INIT;
			}
			else if (!b0 && b1 && number > 0) {
				ScreenState = DEC;
			}
			else{
				ScreenState = WAIT;
			}
			break;
			
		case WAIT:
			if (b0 && !b1 && number < 9) {
				ScreenState = INC;
			}
			else if (b1 && !b0 && number > 0) {
				ScreenState = DEC;
			}
			else if ((!b0 && !b1) || (b0 && !b1 && number >= 9) || (b1 && !b0 && number == 0)) {
				ScreenState = WAIT;	
			}
			else {
				ScreenState = INIT;
			}
			break;
	}
	
	switch (ScreenState){
		case STATE_Start:
			break;
		
		case INIT:
			number = 0;
			LCD_Cursor(1);
			LCD_WriteData(number + '0');
			break;
			
		case INC:
			++number;
			LCD_Cursor(1);
			LCD_WriteData(number + '0');	
			break;
			
		case DEC:
			--number;
			LCD_Cursor(1);
			LCD_WriteData(number + '0');
			break;
		
		case WAIT:
			break;
	}
}

int main()
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	ScreenState = STATE_Start;
	TimerSet(1000);
	TimerOn();
	LCD_init();
    while (1) 
    {
		tick();
		while (!TimerFlag);
		TimerFlag = 0;	
    }
}