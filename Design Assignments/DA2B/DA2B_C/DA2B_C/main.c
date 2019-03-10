/* DA2B_C.c
 * Created: 3/9/2019 3:45:18 PM
 * Author : John
 */ 

#define	 F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main()
{
	DDRB |= 1<<2;				//Set PortB.2 as output
	PORTD |= 1<<2;				//Pull-up activated
	EICRA = 0x2;				//Make INT0 falling edge triggered
	
	EIMSK = (1<<INT0);			//Enable external interrupt 0
	sei();						//Enable interrupts

    while (1) 
    {
		PORTB |= (1<<2);		//Keeps LED turned off
    }
}

ISR (INT0_vect)					//ISR for external interrupt 0
{
	PORTB &= (0<<2);			//Turns on LED
	_delay_ms(1250);			//Delay for 1.250 seconds

}

