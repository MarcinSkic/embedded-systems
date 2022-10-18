/*
 * Ex3Task1.c
 *
 * Created: 10.10.2022 14:22:10
 * Author : Student_PL
 */ 
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA |= 0xFF;
	PORTA &= 0;
    /* Replace with your application code */
    while (1) 
    {
		//Example 1
		PORTA &= 0;
		PORTA |= 0b11000000;
		_delay_ms(500);
		for(int i = 6; i > 0; i--){
			PORTA ^= 1<<(i-1);
			PORTA ^= 1<<(i+1);
			_delay_ms(500);
		}
		
		for(int i = 1; i < 7; i++){
			PORTA ^= 1<<(i-1);
			PORTA ^= 1<<(i+1);
			_delay_ms(500);
		}
		
		//Example 2
		PORTA &= 0;
		for(int i = 7; i > 3; i--){
			PORTA |= 1<<i;
			_delay_ms(1000);
		}
		
		for(int i = 4; i < 7; i++){
			PORTA &= ~(1<<i);
			_delay_ms(1000);
		}
		
		//Example 3
		PORTA &= 0;
		for(int i = 7; i >= 0;i--){
			PORTA |= 1<<i;
			_delay_ms(1000);
		}
		
		//Example 4
		
		PORTA &= 0;
		for(int i = 3; i >= 0; i--){
			PORTA |= 1<<i;
			_delay_ms(1000);
		}
		
		PORTA &= ~0b00001111;
		for(int i = 4; i<8; i++){
			PORTA |= 1<<i;
			_delay_ms(1000);
		}
		
		//Example 5
		PORTA &= 0;
		for(int i = 7; i > 3; i--){
			PORTA |= 1<<i;
			_delay_ms(1000);
		}
		PORTA &= 0;
		for(int i = 0; i < 4; i++){
			PORTA |= 1<<i;
			_delay_ms(1000);
		}
    }
}
