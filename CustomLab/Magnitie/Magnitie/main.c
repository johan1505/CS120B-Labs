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
enum DetectMetalStates {WaitForMetal, MetalSignal} DetectMetalState;
int DetectMetalTick(int state);

//SM2
enum LEDStates {WaitLED, LEDON} LEDState;
int LEDTick(int state);

int main(void)
{
//	DDRA = 0xFB; PORTA = 0x04; // Setting up the registers to use the screen and the start button
//	DDRD = 0xFF; PORTD = 0x00; //
	DDRA = 0x00; PORTA = 0xFF; // Set up sensor input
	DDRB = 0xFF; PORTB = 0x00; // Output for the LEDS/ mainly for testing purposes
	static task task1;
	static task task2;
	
	// Period for the tasks
	unsigned long int DetectMetalSMPeriod = 50;
	unsigned long int LEDSMPeriod = 50;

	//Calculating GCD
	unsigned long int taskPeriodGCD = findGCD(DetectMetalSMPeriod, LEDSMPeriod);
	
	//Set up of the array of tasks
	task *tasks[] = {&task1, &task2};
	unsigned char numTasks = 2;
	
	//task 1
	task1.state = WaitForMetal;
	task1.period = DetectMetalSMPeriod;
	task1.elapsedTime = task1.period;
	task1.TickFct = &DetectMetalTick;
	
	//task 2s
	task2.state = WaitLED;
	task2.period = LEDSMPeriod;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LEDTick;
	
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
	const unsigned short threshHold = 156;
	switch(state){	
		case WaitForMetal:
			state = ADC <= threshHold ? MetalSignal : WaitForMetal;
			break;
		
		case MetalSignal:	
			state = ADC <= threshHold ? MetalSignal : WaitForMetal;
			break;
			
		default:
			state = WaitForMetal;
			break;
	}
	switch(state){
		case WaitForMetal:
			MetalDetected = 0;
			break;
			
		case MetalSignal:
			MetalDetected = 1;
			break;
		
		default:
			break;	
	}
	return state;
}


int LEDTick(int state){
	switch(state){
		case WaitLED:
			state = MetalDetected ? LEDON : WaitLED;
			break;
		
		case LEDON:	
			state = MetalDetected ? LEDON : WaitLED;
			break;
			
		default: 
			state = WaitLED;
			break;
	}
	
	switch(state){
		case WaitLED:
			PORTB = 0x00;
			break;
			
		case LEDON:
			PORTB = 0xFF;
			break;
		
		default:
			break;
	}
	return state;
}
