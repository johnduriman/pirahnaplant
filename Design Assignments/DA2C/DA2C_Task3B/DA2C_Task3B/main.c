/*
 * DA2C_Task3B.c
 *
 * Created: 3/18/2019 2:16:10 PM
 * Author : John
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

//1250ms == 1,250,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 19,531.25
//Choosing an OVF of 105 -- Divide 19531.25/105 = 186.01 which is approx 186
//OCR0A = 186 = 0xBA

volatile uint8_t OVFCount;
volatile uint8_t LEDControl;
volatile uint8_t PINPressed;

int main(void)
{
	DDRB |= (1<<2);								//Set PortB.2 as an output
	DDRC &= (0<<1);								//Sets PORTC.1 as an input
	PORTC |= (1<<1);							//Enable pull-up
	TCCR0A |= (1<<WGM01);						//Set to CTC Mode
	TCCR0B |= (1<<CS00) | (1<<CS02);			//Set prescaler to 1024
	PORTB |= (1<<2);							//Turn off LED
	OCR0A = 0xBA;								//Sets counter to 0x38
	PINPressed = 0;								//Reset value
	OVFCount = 0;								//Resets OVFCount
	LEDControl = 0;								//Resets LEDControl
	TIMSK0 = (1<<OCIE0A);						//Enable Timer0 Compare Match Interrupt
	sei();										//Enable global interrupts
	TCNT0 = 0;									//Initialize counter
	
	while (1)
	{
		//Checks to see if PINC.1 has been pressed
		if(!(PINC & (1<<PINC1)))
		{
			PINPressed++;						//Lets interrupt know pin has been pressed
			TCNT0 = 0;							//Reset counter
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	//The switch must be pressed first before this code runs
	if(PINPressed > 0)
	{
		//First part of loop will turn the LED on
		if(LEDControl == 0)
		{
			PORTB &= ~(1<<2);						//Turns on LED
			if(OVFCount == 105)						//1250ms
			{
				OVFCount = 0;						//Resets OVFCount
				LEDControl++;						//LEDControl will turn it off
			}
			OVFCount++;								//Increments OVFCount
			TCNT0 = 0;								//Resets counter to 0x38
		}
		else
		{
			PORTB |= (1<<2);						//Turns off LED
			LEDControl--;							//Resets LEDControl
			PINPressed = 0;							//Resets state of PINPressed
		}
	}
	else
	{
		TCNT0 = 0;
	}
}


