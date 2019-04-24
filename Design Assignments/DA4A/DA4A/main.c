#define F_CPU 16000000UL
#define BAUD 9600
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

//Function declarations
void read_adc(void);
void adc_init(void);
void USART_init(void);
void USART_tx_string(char *data);
unsigned char USART_Receive(void);

volatile uint8_t potValue;

int main(void)
{
	char buffer[20];
	//Setup inputs and outputs
	DDRB = 0xFF;								//Set Port B as output
	DDRD |= (1<<5);								//OC0B
	PORTD |= (1<<7);							//Enable pull-up
	
	//Setup timer
	TCCR0A	|= (1<<WGM00)						//Fast PWM
			| (1<<WGM01)						//Fast PWM
			| (1<<COM0A1)						//Non Inverting
			| (1<<COM0B1);						//Non Inverting
	TCCR0B	|= (1<<CS01);						//Prescaler = 8
			
	//Setup interrupt
	PCMSK1 |= (1<<PCINT9);						//PC1 
	PCICR |= (1<<PCIE1);						//Any change on PCINT14...8 will cause an interrupt
	sei();										//Enable global interrupts
	
	USART_init();
	adc_init();
	
	TCNT0 = 0;
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);					//Start conversion
		while((ADCSRA & (1<<ADIF)) == 0);		//Wait for conversion to finish
		ADCSRA |= (1<<ADIF);
		
		potValue = ADC;
		if(potValue > 242)						//95% duty cycle of 255 is 242
		{
			OCR0B = 242;
		}
		else									//set OCR0A to potentiometer input
		{
			OCR0B = potValue;
		}
		itoa(potValue, buffer, 10);				//Here 10 means decimal
		USART_tx_string(buffer);				//Prints out integer
		USART_tx_string("\n");					//Prints out
		
		_delay_ms(1000);
    }
	
}

ISR(PCINT1_vect)
{
	PCICR |= (0<<PCIE1);
	_delay_ms(100);
	USART_tx_string("Hello");
	PORTB ^= (1<<0); 
	PCICR |= (1<<PCIE1);
}

void adc_init(void)
{
	ADMUX = (0<<REFS1)	|						//Reference Selection Bits
			(1<<REFS0)	|						//AVcc - external cap at AREF
			(0<<ADLAR)	|						//ADC Left Adjust Result
			(0<<MUX2)	|						//Analog Channel Selection bits
			(0<<MUX1)	|						//PC0 | PCINT8 | ADC0
			(0<<MUX0)	;
	
	ADCSRA =	(1<<ADEN)	|					//ADC Enable
				(0<<ADSC)	|					//ADC Start Conversion
				(0<<ADATE)	|					//ADC Auto Trigger Enable
				(0<<ADIF)	|					//ADC Interrupt Flag
				(0<<ADIE)	|					//ADC Interrupt Enable
				(1<<ADPS2)	|					//ADC Prescalar Select Bits
				(0<<ADPS1)	|
				(1<<ADPS0)	;
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

