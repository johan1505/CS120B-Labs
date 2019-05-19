/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 11  Exercise 3
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

//Tick1
enum KeyPadStates {keyPadOutput} KeyPadState;
int KeyPadTick(int state);

enum LCDStates {OUTPUT} LCDState;
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
	task2.state = OUTPUT;
	task2.period = systemPeriod;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LCDTick;
	
	TimerSet(systemPeriod);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
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
	static unsigned char temp = ' ';
	switch(state){
		case OUTPUT:
			if (c == '\0'){
				LCD_Cursor(1);
				LCD_WriteData(temp);
			}
			else {
				temp = c;
				LCD_Cursor(1);
				LCD_WriteData(temp);
			}
			state = OUTPUT;
			break;
		default:
			state = OUTPUT;
			break;
	}
	return state;
}