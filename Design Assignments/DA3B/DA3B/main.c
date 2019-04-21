#define F_CPU 16000000UL
#define BAUD_RATE 9600
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

void usart_init(void);
void adc_init(void);
void read_adc(void);
void usart_tx_string(char * data);
void usart_send(unsigned char ch);
void usart_print(char* str);

volatile uint8_t a;
volatile unsigned int adc_temp;



int main(void)
{
	usart_init();
	adc_init();
	usart_tx_string("Connected!\r\n");
	
	TCCR1B	|= (1<<WGM12)						//CTC mode 
			| (1<<CS10)
			| (1<<CS12);						//1024 prescaler
	OCR1A = 15625;								//1 second
	TIMSK1 |= (1<<OCIE1A);						//Output compare A match interrupt enable
	
	sei();										//Enable global interrupts
	
	TCNT1 = 0;									//Reset timer
	
	while (1)
	{
		ADCSRA |= (1<<ADSC);					//Start conversion
		while((ADCSRA & (1<<ADIF)) == 0);		//Wait for conversion to finish
		ADCSRA |= (1<<ADIF);
		
		//Calculates temperature
		a = ADCL;
		a = a | (ADCH<<8);
		a = (a/1024.0) * 5000/10;
		a = (a * (9/5) + 20);
		
		
		//_delay_ms(1000);		
	}
	
	return 0;
}

ISR(TIMER1_COMPA_vect)
{
	usart_send((a/100) + '0');
	a = a % 100;
	usart_send((a/10) + '0');
	a = a % 10;
	usart_send(a + '0');
	usart_send('\r');
}

void read_adc(void)
{
	unsigned char i = 4;
	adc_temp = 0;
	while(i--)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp += ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp/4;
}

void usart_init(void)
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void adc_init(void)
{
	ADMUX = (0<<REFS1)	|						//Reference Selection Bits
			(1<<REFS0)	|						//AVcc - external cap at AREF
			(0<<ADLAR)	|						//ADC Left Adjust Result
			(1<<MUX2)	|						//Analog Channel Selection bits
			(0<<MUX1)	|						//PIN28 (PC5)
			(1<<MUX0)	;
	
	ADCSRA =	(1<<ADEN)	|					//ADC Enable
				(0<<ADSC)	|					//ADC Start Conversion
				(0<<ADATE)	|					//ADC Auto Trigger Enable
				(0<<ADIF)	|					//ADC Interrupt Flag
				(0<<ADIE)	|					//ADC Interrupt Enable
				(1<<ADPS2)	|					//ADC Prescalar Select Bits
				(0<<ADPS1)	|
				(1<<ADPS0)	;
}

void usart_send(unsigned char ch)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}

void usart_print(char* str)
{
	int i = 0;
	while(str[i] != 0)
	{
		usart_send(str[i]);
	}
}

void usart_tx_string(char *data)
{
	while((*data != '\0'))
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
	
}