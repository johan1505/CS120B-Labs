/*
 * jguzm022_lab1_part1.c
 *
 * Created: 4/3/2019 8:09:16 AM
 * Author : joan1
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
  DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
  DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
  unsigned char tmpB = 0x00; // Temporary variable to hold the value of B0 - output light
  unsigned char tmpA0 = 0x00; // Temporary variable to hold the value of A0 - Door opened?
  unsigned char tmpA1 = 0x00; // Temporary variable to hold the value of A1 - Light sensed?
  while(1)
  {
	  // 1) Read input
	  tmpA0 = PINA & 0x01;
	  tmpA1 = PINA & 0x02;
	  // 2) Perform computation
	  // if PA0 is 1 - door is opened and PA1 is 0 - is night time, set PB0 = 1, else = 0
	  if ((tmpA0 == 0x01) && (tmpA1 == 0x00) ) { // True if PA0 is 1 and PA1 is 0
		  tmpB = (tmpB & 0xFE) | 0x01; // Sets tmpB to bbbbbbb1
		  // (clear rightmost 1 bit, then set to 1)
	  }
	  else {
		  tmpB = (tmpB & 0xFE); // Sets tmpB to bbbbbbb0
		  // (clear rightmost 1 bit)
	  }
	  // 3) Write output
	  PORTB = tmpB;
  }
  return 0;
}

