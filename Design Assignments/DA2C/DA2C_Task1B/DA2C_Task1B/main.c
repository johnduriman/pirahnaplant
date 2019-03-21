/*
 * DA2C_Task1B.c
 *
 * Created: 3/17/2019 3:23:50 PM
 * Author : John
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>

//1250ms == 1,250,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 19,531.25
//Choosing an OVF of 105 -- Divide 19531.25/105 = 186.01 which is approx 186
//TCNT0 = 256 - 186 = 70 = 0x46

int main(void)
{
	uint8_t OVFCount = 0;						//Initialize OVFCount
	DDRB |= (1<<2);								//Set PortB.2 as an output
	DDRC &= (0<<1);								//Sets PORTC.1 as an input
	PORTC |= (1<<1);							//Enable pull-up
	TCCR0A = 0X00;								//Set to Normal Mode
	TCCR0B |= (1<<CS00) | (1<<CS02);			//Set prescaler to 1024
	
	while (1)
	{
		//Check if PINC.1 has been pressed
		if(!(PINC & (1<<PINC1)))
		{
			//Keeps LED on for 435ms
			for(OVFCount = 0; OVFCount <= 105; OVFCount++)
			{
				PORTB &= ~(1<<2);					//Turns on LED
				TCNT0 = 0x46;						//Resets counter to 0x38
				while ((TIFR0 & 0x01) == 0);		//Check for flag
				TIFR0 = 0x01;						//Resets overflow flag
			}
		}
		//Keep default state of LED.2 to off
		else
		{
			PORTB |= (1 << 2);					//Turns off LED
		}
	}	
}



