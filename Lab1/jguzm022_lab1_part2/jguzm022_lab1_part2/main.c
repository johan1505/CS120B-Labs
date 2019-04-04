/*
 * jguzm022_lab1_part2.c
 *
 * Created: 4/3/2019 1:43:05 PM
 * Author : johan */ 

#include <avr/io.h>

/*Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is parked in the space, of a four-space parking lot.
 Write a program that outputs in binary on port C the number of available spaces (Hint: declare a variable "unsigned char cntavail"; 
 you can assign a number to a port as follows:
PORTC = cntavail;).*/

int main(void)
{
   DDRA = 0x00; PORTA = 0x06; // Configure port A's 8 pins as inputs
   DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
   unsigned char tmpAx = 0x00; // Temporary variable to hold the value of A0
   //unsigned char tmpA1 = 0x00; // Temporary variable to hold the value of A1
   //unsigned char tmpA2 = 0x00; // Temporary variable to hold the value of A2
   //unsigned char tmpA3 = 0x00; // Temporary variable to hold the value of A3
   unsigned char cntavail;
   while (1)
   {
	   cntavail = 0x00;
	   for(unsigned char i = 0x01; i <= 0x08 ; i = 2*i){
		   tmpAx = PINA & i;
		   if (tmpAx == 0x00){
			   ++ cntavail;
		   }
	   }
	   
	   PORTC = cntavail;
   }
   
   return 0;
}