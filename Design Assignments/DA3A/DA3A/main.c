/* DA3A.c
 * Created: 3/27/2019 12:35:07 AM
 * Author : John Duriman
 */ 

//1000 ms == 1,000,000 us
//Divide by .0625 (16MHz) and 1024 prescaler we get 15625
//Choosing an OVF of 125 -- Divide 15625 = 125 
//TCNT0 = 255 - 125 = 130 = 0x82

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)
#define UBRR_9600 103							//for 16 Mhz with .2% error
#define RAND_MAX 0x7fffffff

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

//Function declarations
void read_adc(void);
void adc_init(void);
void USART_init(void);
void USART_tx_string(char *data);
unsigned char USART_Receive(void);
volatile unsigned int adc_temp;					//Global variable unsigned int
volatile uint8_t OVFCount;
volatile uint8_t Delay;
volatile uint8_t r;								

char buffer[20];


int main(void)
{
	adc_init();									//Initialize the ADC
	USART_init();								//Initialize the USART
	TCCR0A = 0X00;								//Set to Normal Mode
	TCCR0B |= (1<<CS00) | (1<<CS02);			//Set prescaler to 1024
	OVFCount = 0;								//Resets OVFCount
	Delay = 0;									//Resets Delay
	TIMSK0 |= (1<<TOIE0);						//Enable Timer0 Overflow Interrupt
	sei();										//Enable global interrupts
	TCNT0 = 0x82;								//Resets counter to 0x82
    while (1) {}								//Do nothing
}

void USART_init(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);			//8 bit data
	UCSR0B = _BV(RXEN0)  | _BV(TXEN0);			//Enable RX and TX
}

void USART_tx_string(char *data)
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *data;
		data++;
	}
}

unsigned char USART_Receive(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
	
}

void adc_init(void)
{
	/** Setup and enable ADC **/
	ADMUX = (0 << REFS1)	|					//Reference Selection Bits	
			(1 << REFS0)	|					//AVcc - external cap at AREF
			(0 << ADLAR)	|					//ADC Left Adjust Result
			(0 << MUX2)		|					//ANalog Channel Selection Bits
			(1 << MUX1)		|					//ADC2 (PC2 PIN25)
			(1 << MUX0)		;
			
	ADCSRA =	(1 << ADEN)	|					//ADC Enable
				(0 << ADSC)	|					//ADC Start Conversion
				(0 << ADATE)|					//ADC Auto Trigger Enable
				(0 << ADIF)	|					//ADC Interrupt Flag
				(0 << ADIE)	|					//ADC Interrupt Enable
				(1 << ADPS2)|					//ADC Prescaler Select Bits
				(0 << ADPS1)|
				(1 << ADPS0);					//Select channel
									
	
}

ISR(TIMER0_OVF_vect)
{
	if(Delay == 0)
	{
		if(OVFCount == 125)
		{
			OVFCount = 0;						//Reset OVFCount
			Delay = 1;							//Go to else
		}
		OVFCount++;								//Increment overflow count
		TCNT0 = 0x82;							//Resets counter to 0x82
	}
	else
	{
		r = rand();								//Returns a random integer
		USART_tx_string("Integer: ");			//Prints out string
		itoa(r, buffer, 10);					//Here 10 means decimal
		USART_tx_string(buffer);				//Prints out integer
		USART_tx_string("\n");					//Prints out
		
		r = rand();								//Returns a random integer
		itoa(r, buffer, 10);					//Here 10 means decimal
		USART_tx_string("Float  : ");			//Prints out string
		USART_tx_string(buffer);				//Prints out integer
		USART_tx_string(".");					
		r = rand();								//Returns a random integer
		itoa(r, buffer, 10);					//Here 10 means decimal
		USART_tx_string(buffer);				//Prints out integer
		USART_tx_string("\n");					//Enter
		
		Delay = 0;								//Execute if statement
		TCNT0 = 0x82;							//Resets counter to 0x82

	}
	
}