#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>


unsigned int seconds=0,mins=0,hours=0,sec2=0,min2=0,hr2=0;
void INT_Init()
{
	MCUCR=(1<<ISC01)|(1<<ISC10)|(1<<ISC11);
	MCUCSR&=~(1<<ISC2);
	GICR=(1<<INT2)|(1<<INT1)|(1<<INT0);
	DDRD  &= (~(1<<PD3));
	DDRD  &= (~(1<<PD2));
	DDRB  &= (~(1<<PB2));
}
void Timer1_Init()
{
	TCCR1A=(1<<FOC1A);
	TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
	TCNT1=0;
	OCR1A=1000;
	TIMSK=(1<<OCIE1A);

}

ISR(INT0_vect)
{
	TCNT1=0;
	seconds=0;
	mins=0;
	hours=0;
	sec2=0;
	min2=0;
	hr2=0;

}

ISR(INT1_vect)
{
	TCCR1B&=~(1<<WGM12);
}

ISR(INT2_vect)
{

	Timer1_Init();
}


ISR(TIMER1_COMPA_vect)
{
	seconds++;
	if(seconds>9)
	{
		seconds=0;
		sec2++;
	}
	if(sec2==6&&seconds==0)
	{
		sec2=0;
		mins++;

	}
	if(mins>9)
	{
		min2++;
		mins=0;
	}
	if(min2==6&&mins==0)
	{
		min2=0;
		hours++;

	}
	if (hours>9)
	{
		hr2++;
		hours=0;
	}

}

int main()
{
	DDRC|=0x0F;
	PORTC&=0xF0;
	SREG|= (1<<7);
	INT_Init();
	Timer1_Init();
	unsigned int sec1=0,min1=0,hr1=0;
	while(1)
	{
		sec1=(seconds&0x0F);
    	PORTA=0x01;
		PORTC=sec1;     //First digit from Seconds
		_delay_ms(5);

		PORTA=0x02;
		PORTC=sec2;    //Second digit from Seconds
		_delay_ms(5);

		min1=mins&0x0F;
		PORTA=0x04;
		PORTC=min1;   //First digit from minutes
		_delay_ms(5);

		PORTA=0x08;
		PORTC=min2;   //Second digit from minutes
		_delay_ms(5);

		hr1=hours&0x0F;

		PORTA=0x10;
		PORTC=hr1;
		_delay_ms(5);

		PORTA=0x20;
		PORTC=hr2;       //Second digit from hours
		_delay_ms(5);


	}
}
