/*
 * Lab3_LCD_DRIVER.c
 *
 * Created: 24.10.2022 14:07:10
 * Author : Student_PL
 */ 
#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

int main(void){
	LCD_Startup();
	LCD_writeText("Adam",4);
	LCD_writeText(" umie",5);
	LCD_writeText(" grac",5);
	LCD_moveCursor(1,0);
	LCD_writeText("Usun: MNIE",10);
	_delay_ms(1000);
	LCD_clearFromPosition(1,6);
}
