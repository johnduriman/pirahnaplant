/*
 * DA2C_Task2B.c
 *
 * Created: 3/17/2019 6:56:06 PM
 * Author : John
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

//1250ms == 1,250,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 19,531.25
//Choosing an OVF of 105 -- Divide 19531.25/105 = 186.01 which is approx 186
//TCNT0 = 256 - 186 = 70 = 0x46

volatile uint8_t OVFCount;
volatile uint8_t LEDControl;
volatile uint8_t PINPressed;

int main(void)
{
	DDRB |= (1<<2);								//Set PortB.2 as an output
	DDRC &= (0<<1);								//Sets PORTC.1 as an input
	PORTC |= (1<<1);							//Enable pull-up
	TCCR0A = 0X00;								//Set to Normal Mode
	TCCR0B |= (1<<CS00) | (1<<CS02);			//Set prescaler to 1024
	PORTB |= (1<<2);							//Turn off LED
	PINPressed = 0;								//Reset value
	OVFCount = 0;								//Resets OVFCount
	LEDControl = 0;								//Resets LEDControl
	TIMSK0 |= (1<<TOIE0);						//Enable Timer0 Overflow Interrupt
	sei();										//Enable global interrupts
	
	while (1)
	{
		//Checks to see if PINC.1 has been pressed
		if(!(PINC & (1<<PINC1)))
		{
			TCNT0 = 0x46;						//Reset Timer counter
			PINPressed++;						//Allows for the timer to execute the code
		}
	}
}

ISR(TIMER0_OVF_vect)
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
			TCNT0 = 0x46;							//Resets counter to 0x38
		}
		//After OVFCount reaches 105, turn off LED
		else
		{
			PORTB |= (1<<2);						//Turns off LED
			LEDControl--;							//Resets LEDControl
			PINPressed = 0;							//Resets state of PINPressed
		}
	}
	else
	{
		TCNT0 = 0x46;								//Reset timer counter
	}
}

