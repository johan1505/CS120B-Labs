/*	Partner 1 Name & E-mail: Johan Guzman Avalos - jguzm022@ucr.edu
 *	Partner 2 Name & E-mail: Adrian De La Torre  - adel037@ucr.edu
 *	Lab Section: 25
 *	Assignment: Lab 3  Exercise 3
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
	
  #include <avr/io.h>


    /*In addition to the above, PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated, and PA6 is 1 if the driver's seatbelt is fastened.
	 PC7 should light a "Fasten seatbelt" icon if a key is in the ignition, the driver is seated, but the belt is not fastened. */
  int main(void)
  {
	  DDRA = 0x00; PORTA = 0xFF;
	  DDRC = 0xFF; PORTC = 0x00;
	  unsigned char fuelSensor = 0x00;
	  unsigned char keyInIgnition = 0x00;
	  unsigned char driverSeated  = 0x00;
	  unsigned char seatbeltFastened = 0x00;
	  unsigned char fuelLed    = 0x00;
	  while (1)
	  {
		  PORTC = 0x00;
		  fuelSensor = PINA & 0x0F;
		  keyInIgnition = PINA & 0x10;
		  driverSeated  = PINA & 0x20;
		  seatbeltFastened = PINA & 0x40;
		  fuelLed    = 0x00;
		  
		  if(keyInIgnition && driverSeated && !seatbeltFastened){
			  fuelLed = 0x80;
		  }
		  if(fuelSensor == 0){
			  fuelLed = fuelLed | 0x40;
		  }
		  else if (fuelSensor == 1 || fuelSensor == 2){
			  fuelLed = (fuelLed | 0x20) | 0x40;
		  }
		  else if(fuelSensor == 3 || fuelSensor == 4){
			  fuelLed = (fuelLed | 0x30) | 0x40;
		  }
		  else if(fuelSensor == 5 || fuelSensor == 6){
			  fuelLed = fuelLed | 0x38;
		  }
		  else if(fuelSensor == 7 || fuelSensor == 8 || fuelSensor == 9){
			  fuelLed = fuelLed | 0x3C;
		  }
		  else if(fuelSensor == 10 || fuelSensor == 11 || fuelSensor == 12){
			  fuelLed = fuelLed | 0x3E;
		  }
		  else if(fuelSensor == 13 || fuelSensor == 14 || fuelSensor == 15){
			  fuelLed = fuelLed | 0x3F;
		  }
		  
		  PORTC = fuelLed;
	  }
  }
