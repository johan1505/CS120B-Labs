/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 11  Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#include "io.c"
#include "timer.c"
#include "scheduler.h"

//global variable
unsigned char index = 0;

enum IndextStates {IndexInit, IncreaseIndex} IndexState;
int IndexTick(int state);

enum LCDStates {OUTPUT} LCDState;
int LCDTick(int state);

int main(void)
{
    /* Replace with your application code */
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned long systemPeriod = 500;
	static task task1;
	static task task2;
	task *tasks[] = {&task1, &task2};
	unsigned char numTasks = sizeof(tasks)/sizeof(task*);
	
	//task 1
	task1.state = IndexInit;
	task1.period = systemPeriod;
	task1.elapsedTime = task1.period;
	task1.TickFct = &IndexTick;
	
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

int IndexTick(int state){
	switch(state){
		case IndexInit:
			state = IncreaseIndex;
			break;
		case IncreaseIndex:
			state = (index <= 51) ? IncreaseIndex : IndexInit;
			break;
		default:
			state = IndexInit;
			break;
	}
	
	switch(state){
		case IndexInit:
			index = 0;
			break;
		case IncreaseIndex:
			index++;
			break;
		default:
			index = 0;
			break;
	}
	
	return state;
}

int LCDTick(int state){
	static unsigned char phrase[69] = {' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'C', 'S', '1', '2', '0', 'B', ' ', 'i', 's', ' ', 'L', 'e', 'g', 'e', 'n', 'd', '.', '.', '.', ' ', 'w', 'a', 'i', 't', ' ', 'f', 'o', 'r', ' ', 'i', 't', ' ', 'D', 'A', 'R', 'Y', '!' ,' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
	static unsigned char k, temp;
	temp = index;
	switch(state){
		case OUTPUT:
			state = OUTPUT;
			break;
		default:
			state = OUTPUT;
			break;
	}
	
	switch(state){
		case OUTPUT:
			for (k = 1; k <= 16; k++){
				LCD_Cursor(k);
				LCD_WriteData(phrase[temp]);
				temp++;
			}
			break;
		default:
			break;
	}
	
	return state;
}