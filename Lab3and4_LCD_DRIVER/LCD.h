#ifndef LCD_Driver
#define LCD_Driver

#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>
#define LCD_DDR DDRA
#define LCD_PORT PORTA
#define LCD_RS 0
#define LCD_E 1

void LCD_writeUpperBits(char byte);

void LCD_writeByte(char byte);

void LCD_writeInstruction(char instruction);

void LCD_clear();

void LCD_writeChar(char character);

void LCD_writeText(char text[], int length);

void LCD_moveCursor(int row, int column);

void LCD_pisz1();

void LCD_pisz2();

void LCD_xy(int x,int y);

void LCD_clearFromPosition(int row, int column);

void LCD_clr_1();

void LCD_clr_2();

void LCD_clr_xy(int x, int y);

void LCD_Startup();

#endif
