/*
 * DA2C_Task3A.c
 *
 * Created: 3/17/2019 10:11:30 PM
 * Author : John
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

//435ms == 435,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 6796.875
//Choosing an OVF of 34 -- Divide 6795.875/34 = 199.9 which is approx 200
//OCR0A = 200 = 0xC8


//300ms == 300,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 4687.5
//Choosing an OVF of 33 -- Divide 4687.5/33 = 142.04 which is 142
//OCR0A = 142 = 0x8E

volatile uint8_t OVFCount;
volatile uint8_t LEDControl;

int main(void)
{
	DDRB |= (1<<2);								//Set PortB.2 as an output
	TCCR0A |= (1<<WGM01);						//Set to CTC Mode
	TCCR0B |= (1<<CS00) | (1<<CS02);			//Set prescaler to 1024
	TCNT0 = 0;									//Initialize counter
	OCR0A = 0xC8;								//Sets counter to 0x38
	OVFCount = 0;								//Resets OVFCount
	LEDControl = 0;								//Resets LEDControl
	TIMSK0 = (1<<OCIE0A);						//Enable Timer0 Compare Match
	sei();										//Enable global interrupts
	while (1){}
	
}

ISR(TIMER0_COMPA_vect)
{
	if(LEDControl == 0)
	{
		PORTB &= ~(1<<2);						//Turns on LED
		if(OVFCount == 34)
		{
			OVFCount = 0;						//Reset overflow counter
			LEDControl++;						//Switch thte state to LED off
		}
		OVFCount++;								//Increment overflow counter
		OCR0A = 0xC8;							//Assigns 435 ms timer
		TCNT0 = 0;								//Resets counter 
	}
	else
	{
		PORTB |= (1<<2);						//Turns off LED
		if(OVFCount == 33)
		{
			OVFCount = 0;						//Reset overflow counter
			LEDControl--;						//Switch the state to LED on
		}
		OVFCount++;								//Increment overflow counter
		OCR0A = 0x8E;							//Assigns 300 ms timer
		TCNT0 = 0;								//Resets counter
	}
	
}


