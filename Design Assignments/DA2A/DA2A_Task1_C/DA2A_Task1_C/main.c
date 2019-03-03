/*
 * DA2A_Task1_C.c
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

    while (1) 
    {
		PORTB &= ~(1 << 2);		//Turns on LED
		//_delay_ms(435);			//Delay of .435 seconds which is 60% of .735
		PORTB |= (1 << 2);		//Turns off LED
		//_delay_ms(30);			//Delay of .30 seconds which is 40% of .735
    }
	return 0;
}

