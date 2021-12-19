/*
 * Les_11_1_MAL.c
 *
 * Created: 12/18/2021 1:12:03 AM
 * Author : User
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define  SEG7_DDR DDRB
#define  SEG7_PORT PORTB

#define SEG7_DDR_GROUND DDRC
#define SEG7_PORT_GROUND PORTC

int counter_seconds = 0;
int counter_minutes = 0;
int z = 0;//for dynamic indication

int r1_1000 = 0;
int r2_100 = 0;
int r3_10 = 0;
int r4_1 = 0;
int watch_time = 0;


int digits [11] =
{
	0b00111111, //0
	0b00000110, //1
	0b01011011, //2
	0b01001111, //3
	0b01100110, //4
	0b01101101, //5
	0b01111101, //6
	0b00000111, //7
	0b01111111, //8
	0b01101111,  //9
	0b10000000  //dp
};

void chislo(int c_minutes, int  c_seconds )
{
	r1_1000 = c_minutes/10;
	r2_100 = c_minutes%10;
	
	r3_10 = c_seconds/10;
	r4_1 = c_seconds%10;
	
}

ISR(TIMER0_OVF_vect)
{
	if (++z>5) z=0;
	
	if (z == 1)
	{
		SEG7_PORT_GROUND |= (1<<3) | (1<<2) | (1<<1);	// Выключаем 2 и 3 разряды
		SEG7_PORT_GROUND &= ~(1<<0);					// ВКлючаем 1 разряд
		SEG7_PORT = digits [r1_1000];					// Thousands
	}
	else if (z == 2)
	{
		SEG7_PORT_GROUND |= (1<<3)| (1<<2) | (1<<0);	// Выключаем 1 и 3 разряды
		SEG7_PORT_GROUND &= ~(1<<1);					// ВКлючаем 2 разряд
		SEG7_PORT = digits [r2_100];					// Десятки
		 				
	}
	
 	else if (z == 3)
 	{
 		SEG7_PORT_GROUND |= (1<<3)| (1<<2) | (1<<0);	// Выключаем 1 и 3 разряды
 		SEG7_PORT_GROUND &= ~(1<<1);					// ВКлючаем 2 разряд
 		SEG7_PORT = 0b10000000;							//dp;				
 	}
	
	else if (z == 4)
	{
		SEG7_PORT_GROUND |= (1<<3)| (1<<1) | (1<<0);	// Выключаем 1 и 2 разряды
		SEG7_PORT_GROUND &= ~(1<<2);					// ВКлючаем 3 разряд
		SEG7_PORT = digits [r3_10];						// tens
	}
	
	else if (z == 5)
	{
		SEG7_PORT_GROUND |= (1<<2)| (1<<1) | (1<<0);	// Выключаем 1 и 2 разряды
		SEG7_PORT_GROUND &= ~(1<<3);					// ВКлючаем 3 разряд
		SEG7_PORT = digits [r4_1];						// Единицы
	}
	
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 49910;
	
	
	counter_seconds++;
	if (counter_seconds == 59)
	{
		counter_seconds = 0;
		counter_minutes++;
	}
	
	if (counter_minutes == 59)
	{
		counter_minutes = 0;
		
	}

	
}
int main(void)
{
//     DDRD = 0xff;
// 	PORTD = 0x00;
	
	SEG7_DDR = 0xff;
	SEG7_PORT =  0x00;
	
	
// 	DDRC|= (1<<0);//for LED
// 	PORTC&=~(1<<0);
	
	SEG7_DDR_GROUND |=(1<<3) | (1<<2) | (1<<1) | (1<<0);	//for 7seg grounds
	SEG7_PORT_GROUND |=(1<<3) | (1<<2) | (1<<1) | (1<<0);
	
	TCCR1B |= (1<<CS11) | (1<<CS10);						//Prescaler to 64 of TC1 so we after overflow it would be 4.2 sec
	TCCR1B &=~(1<<CS12);
	
	TCNT1 = 49910;
	
	TIMSK |=(1<<TOIE1);										//Enable overflow interrupt TC1
	//sei(); //enable globally

	//Enable Timer Counter 0 with prescaler 8 of 1MHz MCU
	TCCR0 |=(1<<CS01);
	TCCR0 &=~(1<<CS02)|(1<<CS00);
	TCNT0 = 0;
	
	TIMSK |=(1<<TOIE0);//Enable the overflow of TC0
	sei();
	
	
    while (1) 
    {
		
		chislo(counter_minutes, counter_seconds);
			
	
    }
}

