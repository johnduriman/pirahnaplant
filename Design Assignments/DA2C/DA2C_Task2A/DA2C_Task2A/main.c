/*
 * DA2C_Task2A.c
 *
 * Created: 3/17/2019 3:46:36 PM
 * Author : John
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

//435ms == 435,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 6796.875
//Choosing an OVF of 34 -- Divide 6795.875/34 = 199.9 which is approx 200
//TCNT0 = 256 - 200 = 56 = 0x38

//300ms == 300,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 4687.5
//Choosing an OVF of 33 -- Divide 4687.5/33 = 142.04 which is 142
//TCNT0 = 256 - 142 = 114 = 0x72

volatile uint8_t OVFCount;
volatile uint8_t LEDControl;

int main(void)
{
	DDRB |= (1<<2);								//Set PortB.2 as an output
	TCCR0A = 0X00;								//Set to Normal Mode
	TCCR0B |= (1<<CS00) | (1<<CS02);			//Set prescaler to 1024
	TCNT0 = 0x38;								//Sets counter to 0x38
	OVFCount = 0;								//Resets OVFCount
	LEDControl = 0;								//Resets LEDControl
	TIMSK0 |= (1<<TOIE0);						//Enable Timer0 Overflow Interrupt
	sei();										//Enable global interrupts
	while (1){}									//Do nothing
		
}
	
ISR(TIMER0_OVF_vect)
{
	//This if/else switches from LED on and LED off
	if(LEDControl == 0)
	{
		PORTB &= ~(1<<2);						//Turns on LED
		if(OVFCount == 34)
		{
			OVFCount = 0;						//Reset OVFCount
			LEDControl++;						//Switch to LED off
		}
		OVFCount++;								//Increment overflow count
		TCNT0 = 0x38;							//Resets counter to 0x38
	}
	else
	{
		PORTB |= (1<<2);						//Turns off LED
		if(OVFCount == 33)
		{
			OVFCount = 0;						//Reset OVFCount
			LEDControl--;						//Switch to LED on
		}
		OVFCount++;								//Increment overflow count
		TCNT0 = 0x72;							//Resets counter to 0x72
	}
	
}

