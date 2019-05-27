#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <keypad.h>
#include <stdio.h>
#include <scheduler.h>
#include <ADC.h>
#include "io.c"

//Global Variable
unsigned char MetalDetected ;

//SM1
enum DetectMetalStates {DetectMetalStart, WaitForMetal, MetalSignal} DetectMetalState;
int DetectMetalTick(int state);

//SM2
enum LCDStates {LCDInit, SadFace, HappyFace} LCDstate;
int LCDTick(int state);
void InitCustomCharacters(); //Helper

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Set up sensor input
	DDRB = 0xFF; PORTB = 0x00; // Output for the LEDS/ mainly for testing purposes
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	static task task1;
	static task task2;
	
	// Period for the tasks
	unsigned long int DetectMetalSMPeriod = 100;
	unsigned long int LCDPeriod = 1000;

	//Calculating GCD
	unsigned long int taskPeriodGCD = findGCD(DetectMetalSMPeriod, LCDPeriod);
	
	//Set up of the array of tasks
	task *tasks[] = {&task1, &task2};
	unsigned char numTasks = 2;
	
	//task 1
	task1.state = DetectMetalStart;
	task1.period = DetectMetalSMPeriod;
	task1.elapsedTime = task1.period;
	task1.TickFct = &DetectMetalTick;
	
	//task 2s
	task2.state = LCDInit;
	task2.period = LCDPeriod;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LCDTick;
	
	LCD_init();
	ADC_init();
	TimerSet(taskPeriodGCD);
	TimerOn();
	unsigned char i;

    while (1) 
    {
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += taskPeriodGCD; // FIX?
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

int DetectMetalTick(int state){
	static unsigned char i;
	const unsigned short threshHold = 156;
	switch(state){	
		case DetectMetalStart:
			state = WaitForMetal;
			i = 0;
			break;

		case WaitForMetal:
			state = ADC <= threshHold ? MetalSignal : WaitForMetal;
			break;
		
		case MetalSignal:	
			//state = ADC <= threshHold ? MetalSignal : WaitForMetal;
			if (i <= 10 || ADC <= threshHold ){    // Once a metal is detected, rise the metalDetected flag for 1 second so that the LEDSM can read it 
				state = MetalSignal;
			}
			else if (i > 10 && ADC > threshHold){
				state = WaitForMetal;
				i = 0;
			}
			break;
			
		default:
			state = DetectMetalStart;
			break;
	}
	switch(state){
		case DetectMetalStart:
			break;
			
		case WaitForMetal:
			MetalDetected = 0;
			break;
			
		case MetalSignal:
			MetalDetected = 1;
			i++;
			break;
		
		default:
			break;	
	}
	return state;
}


int LCDTick(int state){                   
	static unsigned char i;
	switch(state){
		case LCDInit:
			state = SadFace;
			i = 0;
			InitCustomCharacters();
			break;
	
		case SadFace:
			if (MetalDetected){
				state = HappyFace;
				LCD_Cursor(24);   // Display the happy mouth
				LCD_WriteData(1);
				LCD_Cursor(25);
				LCD_WriteData(2);
			}
			else {
				state = SadFace;
			}
			break;
		
		case HappyFace:	
			if (i <= 5 || MetalDetected){ // Once a metal is detected, keep the LEDs on for 5 second
				state = HappyFace;
			}
			else if (i > 5 && !MetalDetected){
				state = SadFace;
				i = 0;
				LCD_Cursor(24);   // Display the sad mouth
				LCD_WriteData(3);
				LCD_Cursor(25);
				LCD_WriteData(4);
			}
			break;
			
		default: 
			state = LCDInit;
			break;
	}
	
	switch(state){
		case LCDInit:
			break;
			
		case SadFace:
			break;
			
		case HappyFace:
			i++;
			break;
		
		default:
			break;
	}
	return state;
}


void InitCustomCharacters(){                    //================EYE=========================/=============Happy Right Mouth=================/=============Happy Left Mouth==================/============Sad Right Mouth===================/============Sad Left Mouth=======================/
	const  unsigned char customCharacters[40] = {0x1B, 0x15, 0x0E, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x1E, 0x1E, 0x0E, 0x0F, 0x07, 0x00, 0x10, 0x18, 0x0F, 0x0F, 0x0E, 0x1E, 0x1C, 0x00, 0x01, 0x03, 0x1E, 0x1E, 0x1F, 0x18, 0x10, 0x00, 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x03, 0x01, 0x00, 0x1C, 0x1E};
	unsigned char j;
	LCD_WriteCommand(0x40);
	//Loading custom characters
	for (j = 0; j < 40; j++){
		LCD_WriteData(customCharacters[j]);
	}
	//Displaying White spaces of the face
	LCD_Cursor(23);
	LCD_WriteData(0xFF);
	
	LCD_Cursor(26);
	LCD_WriteData(0xFF);
	
	LCD_Cursor(8);
	LCD_WriteData(0xFF);
	
	LCD_Cursor(9);
	LCD_WriteData(0xFF);
	
	//Displaying ears
	LCD_Cursor(6);
	LCD_WriteData(0xF1);
	
	LCD_Cursor(11);
	LCD_WriteData(0xF0);
	
	//Displaying eyes
	LCD_Cursor(7);
	LCD_WriteData(0);
	
	LCD_Cursor(10);
	LCD_WriteData(0);
	LCD_Cursor(24);   // Display the sad mouth
	LCD_WriteData(3);
	LCD_Cursor(25);
	LCD_WriteData(4);
}