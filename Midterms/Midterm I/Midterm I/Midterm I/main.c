#define F_CPU 16000000UL
#define BAUD_RATE 9600
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

void usart_init(void);
void adc_init(void);
void read_adc(void);
void usart_tx_string(char * data);
void usart_send(unsigned char ch);
void usart_print(char* str);

volatile uint8_t a;

int main(void)
{
	usart_init();
	adc_init();
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);					//Start conversion
		while((ADCSRA & (1<<ADIF)) == 0);		//Wait for conversion to finish
		ADCSRA |= (1<<ADIF);
		
		//Calculates temperature
		a = ADCL;
		a = a | (ADCH<<8);
		a = (a/1024.0) * 5000/10;
		a = (a * (9/5)) + 52;
		
		//Sets the MODE for wifi settings (AP or station mode)
		char setMODE[] = "AT+CWMODE=1\r\n";
		usart_print(setMODE);
		_delay_ms(1000);
		
		//Connects the ESP to wifi
		//Will change password after this assignment
		char setWIFI[] = "AT+CWJAP=\"John iPhone\",\"Whatnoway\"\r\n";
		usart_print(setWIFI);
		_delay_ms(1000);

		//Sets up the proper MUX settings
		char setMUX[] = "AT+CIPMUX=0\r\n";
		usart_print(setMUX);
		_delay_ms(1000);
		
		//Connect to thingspeak website
		char initThingSpeak[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n" ;
		usart_print(initThingSpeak);
		_delay_ms(1000);
	
		//Sets up for the GET function with the amount of characters to be sent
		char sendThingSpeak[] = "AT+CIPSEND=80\r\n";
		usart_print(sendThingSpeak);
		_delay_ms(1000);
	
		//Sends temperature value in a to thingspeak
		char getThingSpeak[150];
		snprintf(getThingSpeak, "GET https://api.thingspeak.com/update?api_key=GSP67HM68OPHYUQ9&field1=%d\r\n", a);
		usart_print(getThingSpeak);
		_delay_ms(1000);
	
		//End thing speak connection
		char endThingSpeak[] = "AT+CIPCLOSE\r\n";
		usart_print(endThingSpeak);
		
		//Wait about 15 seconds
		_delay_ms(15000);
    }
}


void usart_init(void)
{
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0L = 8;
}

void adc_init(void)
{
	ADMUX = (0<<REFS1)	|						//Reference Selection Bits
			(1<<REFS0)	|						//AVcc - external cap at AREF
			(0<<ADLAR)	|						//ADC Left Adjust Result
			(0<<MUX2)	|						//Analog Channel Selection bits
			(0<<MUX1)	|						//ADC0 (PC0)
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
		i++;
	}
}