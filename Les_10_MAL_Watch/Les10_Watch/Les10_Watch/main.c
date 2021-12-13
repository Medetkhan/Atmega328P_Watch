/*
 * Les10_Watch.c
 *
 * Created: 12/13/2021 7:59:51 PM
 * Author : User
 */ 

#include <avr/io.h>

#define  SEG7_DDR DDRB
#define  SEG7_PORT PORTB

int counter_seconds = 0;



int digits [10] =
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
	0b01101111  //9
};


int main(void)
{
    //Prescaler 64 for 1TC
    TCCR1B |=(1<<CS11)|(1<<CS10);
    TCCR1B &=~(1<<CS12);
    TCNT1 = 0;
	
	SEG7_DDR = 0xff;
	SEG7_PORT =  0x00;
	
    while (1) 
    {
		if (TCNT1 == 15625 )// 1 sec
		{
			TCNT1 = 0;
			
				
			SEG7_PORT = digits[counter_seconds];
				
			
			counter_seconds++;
			
			if (counter_seconds == 10)
			{
				
				counter_seconds = 0;
				
			}
		}
    }
}

