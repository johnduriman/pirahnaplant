#define F_CPU 16000000UL
#define BAUD 9600
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

//Function declarations
void read_adc(void);
void adc_init(void);
void usart_init(void);
void usart_tx_string(char *data);
unsigned char usart_recieve(void);

volatile uint8_t potValue;

int main(void)
{
	char buffer[20];
	
	//Setup inputs and outputs
	DDRB = 0xFF;						//Make PORTB output
	
	//Setup Timer
	TCCR1A  |= (1<<WGM11)				
			|  (1<<COM1A1)
			|  (1<<COM1B1);				//Non inverted PWM
	TCCR1B	|= (1<<WGM13)				
			|  (1<<WGM12)
			|  (1<<CS11)
			|  (1<<CS10);				//Prescaler = 64 MODE 14(FAST PWM)		
	ICR1 = 4999;
	
	//Setup interrupts
	PCMSK1 |= (1<<PCINT9);
	PCICR |= (1<<PCIE1);
	sei();
	
	usart_init();						//Print values
	adc_init();							//For potentiometer
	
  
    while (1) 
    {
		ADCSRA |= (1<<ADSC);
		while((ADCSRA & (1<<ADIF)) == 0);
		ADCSRA |= (1<<ADIF);
		
		potValue = ADC;

		//0 degrees is at 97
		if(potValue > 0)
		{
			OCR1A = 97 + potValue;
		}
		//90  degrees is at 316
		else if(potValue > 75)
		{
			OCR1A = 316 + potValue;
		}
		//135 degrees is at 425
		else if(potValue > 150)
		{
			OCR1A = 425 + potValue;
			
		}
		//180 degrees is at 535
		//Rounding highest potentiometer values to 180 degrees
		else if(potValue > 225)
		{
			OCR1A = 535;
		}
	
		OCR1A = 97 + potValue;							//0 degrees is at 97
		itoa(potValue, buffer, 10);
		usart_tx_string(buffer);
		usart_tx_string("\n");
		_delay_ms(1000);
    }
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

void usart_init(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);			//8 bit data
	UCSR0B = _BV(RXEN0)  | _BV(TXEN0);			//Enable RX and TX
}

void usart_tx_string(char *data)
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *data;
		data++;
	}
}

