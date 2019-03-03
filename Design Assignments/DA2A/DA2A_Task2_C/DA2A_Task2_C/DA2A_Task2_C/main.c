/*
 * DA2A_Task2_C.c
 *
 * Created: 3/2/2019 12:00:48 AM
 * Author : John Duriman
 */ 
#define F_CPU 16000000UL		//16 MHz
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << 2);			//Sets PORTB.2 as an output
	PORTB |= (1 << 2);			//Turn off LED
	DDRC &= (0 << 1);			//Sets PORTC.1 as an input
	PORTC |= (1 << 1);			//Enable pull-up
    while (1) 
    {
		if(!(PINC & (1 << PINC1)))
		{
			PORTB &= ~(1 << 2);		//Turns on LED
			_delay_ms(1250);		//Delay of 1.25 seconds 

		}
		else
		{
			PORTB |= (1 << 2);		//Turns off LED

		}
		_delay_ms(29);			//dELAY OF .29 SECONDS WHICH IS 40% OF .735
    }
	return 0;
}
