/*
 * Lab6_LCD_2.c
 *
 * Created: 2022-06-15 08:06:38
 * Author : Student
 */ 
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "keyboard.h"
#include "led.h"


int main(void)
{
	SW_Conf();
	
	for(int i = 0; i < 8; i++){
		LED_Conf(i);
	}
	
	PORTA = 0;
    
    while (1) 
    {
		int temp = SW_Read();
		for(int i = 0; i < 4; i++){
			if(temp & 1<<i){
				LED_Set(i);
			} else {
				LED_Clear(i);
			}
		}
    }
}

