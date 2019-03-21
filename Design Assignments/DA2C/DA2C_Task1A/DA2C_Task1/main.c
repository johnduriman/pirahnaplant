/*DA2C_Task1.c
 *
 * Created: 3/15/2019 9:25:41 PM
 * Author : John
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>

//435ms == 435,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 6796.875
//Choosing an OVF of 34 -- Divide 6795.875/34 = 199.9 which is approx 200
//TCNT0 = 256 - 200 = 56 = 0x38


//300ms == 300,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 4687.5
//Choosing an OVF of 33 -- Divide 4687.5/33 = 142.04 which is 142
//TCNT0 = 256 - 142 = 114 = 0x72


int main(void)
{
	uint8_t OVFCount = 0;						//Initializes OVFCount and sets to 0
	DDRB |= (1<<2);								//Set PortB.2 as an output
	TCCR0A = 0X00;								//Set to Normal Mode
	TCCR0B |= (1<<CS00) | (1<<CS02);			//Set prescaler to 1024
	
    while (1) 
    {	
		//Keeps LED on for 435ms
		for(OVFCount = 0; OVFCount <= 34; OVFCount++)
		{
			PORTB &= ~(1<<2);					//Turns on LED
			TCNT0 = 0x38;						//Resets counter to 0x38
			while ((TIFR0 & 0x01) == 0);		//Check for flag
			TIFR0 = 0x01;						//Resets overflow flag
		}
		
		//Keeps LED off for 300ms
		for(OVFCount = 0; OVFCount <= 33; OVFCount++)
		{
			PORTB |= (1<<2);					//Turns off LED
			TCNT0 = 0x72;						//Resets counter to 0x72
			while ((TIFR0 & 0x01) == 0);		//Check for flag
			TIFR0 = 0x01;						//Resets overflow flag
		}
    }
	
	
}

