/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 11  Exercise 4
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <keypad.h>
#include <stdio.h>
#include <scheduler.h>
#include "io.c"

//global variable
unsigned char c;
enum KeyPadStates {keyPadOutput} KeyPadState;
int KeyPadTick(int state);

enum LCDStates {wait, UpdateChar, KeepChar} LCDState;
int LCDTick(int state);

int main(void)
{
    DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	
	unsigned long systemPeriod = 250;
	static task task1;
	static task task2;
	task *tasks[] = {&task1, &task2};
	unsigned char numTasks = sizeof(tasks)/sizeof(task*);
	
	//task 1
	task1.state = keyPadOutput;
	task1.period = systemPeriod;
	task1.elapsedTime = task1.period;
	task1.TickFct = &KeyPadTick;
	
	//task 2
	task2.state = wait;
	task2.period = systemPeriod;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LCDTick;
	
	TimerSet(systemPeriod);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	
	LCD_DisplayString(1, "Congratulations!");
	unsigned char i;
   while(1) {
	   // Scheduler code
	   for ( i = 0; i < numTasks; i++ ) {
		   // Task is ready to tick
		   if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
			   // Setting next state for task
			   tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			   // Reset the elapsed time for next tick.
			   tasks[i]->elapsedTime = 0;
		   }
		   tasks[i]->elapsedTime += systemPeriod;
	   }
	   while(!TimerFlag);
	   TimerFlag = 0;
   }
	return 0;
}

int KeyPadTick(int state){
	switch(KeyPadState){
		case keyPadOutput:
			c = GetKeypadKey();
			state = keyPadOutput;
			break;
	}
	return state;
}

int LCDTick(int state){
	static unsigned char cursor;
	static unsigned temp;
	switch(state){	
		case wait:
			if (c == '\0'){
				state = wait;
			}
			else {
				state = UpdateChar;
				cursor = 1;
			}
			break;
			
		case UpdateChar:
			state = c != '\0' ? UpdateChar : KeepChar;
			break;
		
		case KeepChar:
			if (c == '\0'){
				state = KeepChar;
			}
			else if (c != '\0' && cursor < 16){
				state = UpdateChar;
				cursor++;
			}
			else if (c != '\0' && cursor >= 16){
				state = UpdateChar;
				cursor = 1;
			}
			break;
			
		default:
			state = wait;
			break;
	}
	
	switch(state){	
		case wait:
			break;
		
		case UpdateChar:
			temp = c;
			LCD_Cursor(cursor);
			LCD_WriteData(c);
			break;
			
		case KeepChar:
			LCD_Cursor(cursor);
			LCD_WriteData(temp);
			break;
		
		default:
			break;
	}
	return state;
}