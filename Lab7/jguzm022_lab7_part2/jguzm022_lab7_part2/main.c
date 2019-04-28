/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 7  Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

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

enum LED_States {LED_Start, START, ON1, ON2, ON3, PRESSB, WAIT, VICTORY} LED_State;
enum directions {LEFT, RIGHT} direc;
unsigned char b = 0x00;
unsigned char score = 0x00;
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
				if (PORTB == 0x02 && score < 9){
					LCD_Cursor(1);
					++score;
					LCD_WriteData(score + '0');
				}
				else if (PORTB != 0x02 && score > 0){
					LCD_Cursor(1);
					--score;
					LCD_WriteData(score + '0');
				}
				else {
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
				}
	
			}
			else {
				LED_State = ON2;
			}
			break;
		
		case ON2:
			if (direc == LEFT && !b) {
				LED_State = ON3;
			}
			else if (direc == RIGHT && !b){
				LED_State = ON1;
			}
			else {
				LED_State = PRESSB;
				if (PORTB == 0x02 && score < 9){
					LCD_Cursor(1);
					++score;
					LCD_WriteData(score + '0');
				}
				else if (PORTB != 0x02 && score > 0){
					LCD_Cursor(1);
					--score;
					LCD_WriteData(score + '0');
				}
				else {
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
				}
			}
			break;
		
		case ON3:
			if (b){
				LED_State = PRESSB;
				if (PORTB == 0x02 && score < 9){
					LCD_Cursor(1);
					++score;
					LCD_WriteData(score + '0');
				}
				else if (PORTB != 0x02 && score > 0){
					LCD_Cursor(1);
					--score;
					LCD_WriteData(score + '0');
				}
				else {
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
				}
			}
			else {
				LED_State = ON2;
			}
			break;
		
		case PRESSB:
			if (b  && score < 9){
				LED_State = PRESSB;
			}
			else if (!b && score < 9) {
				LED_State = WAIT;
			}
			else if ((b || !b) && score == 9){
				LED_State = VICTORY;
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
		
		case VICTORY:
			if (!b) {
				LED_State = VICTORY;
			}
			else {
				LCD_ClearScreen();
				LCD_Cursor(1);
				LED_State = START;
			}
			break;
		
		default:
			LED_State = LED_Start;
			break;
	}
	
	switch (LED_State){
		case LED_Start:
			break;
			
		case START:
			PORTB = 0x00;
			score = 5;
			LCD_WriteData(score + '0');
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
		
		case VICTORY:
			LCD_DisplayString(1, "VICTORY");
			PORTB = score % 2 ? 0x07 : 0;
			score++;
			break;
			
		default:
			break;
	}
}

int main()
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LED_State = LED_Start;
	TimerSet(300);
	TimerOn();
	LCD_init();
	while (1)
	{
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

