#include "LCD.h"

void LCD_writeUpperBits(char byte){
	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT = (byte & 0xF0) | (LCD_PORT & 0x0F);
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_ms(1);
}

void LCD_writeByte(char byte){
	_delay_ms(1);

	LCD_PORT |= (1<<LCD_E);	//Poinformowanie LCD o rozpoczêciu transmisji
	_delay_ms(1);	//OpóŸnienia ¿eby wyœwietlacz mia³ czas przetworzyæ informacje
	LCD_PORT = (byte & 0xF0) | (LCD_PORT & 0x0F); //Przes³anie 4 starszych bitów danych z zachowaniem aktualnych ustawieñ wyœwietlacza (bitu E i RS) dziêki sumie
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E); //Poinformowanie LCD o zakoñczeniu transmisji

	_delay_ms(1);

	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT = ((byte & 0x0F)<<4) | (LCD_PORT & 0x0F);	//Przes³anie 4 m³odszych bitów danych (u¿ywaj¹c przesuniêcia na pozycjê 4 starszych) z zachowaniem aktualnych ustawieñ wyœwietlacza (bitu E i RS) dziêki sumie
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);

	_delay_ms(1);
}

void LCD_writeInstruction(char instruction){
	LCD_PORT &= ~(1<<LCD_RS);

	LCD_writeByte(instruction);

	LCD_PORT |= 1<<LCD_RS;
}

void LCD_clear(){
	LCD_writeInstruction(0x01);
}

void LCD_writeChar(char character){
	LCD_writeByte(character);
}

void LCD_writeText(char text[], int length){
	for(int i = 0; i < length; i++){
		LCD_writeChar(text[i]);
	}
}

void LCD_moveCursor(int row, int column){
	LCD_writeInstruction(0b10000000 | (row*0x40+column));
}

void LCD_pisz1(){
	LCD_moveCursor(0,0);
}

void LCD_pisz2(){
	LCD_moveCursor(1,0);
}

void LCD_xy(int x,int y){
	LCD_moveCursor(x,y);
}

void LCD_clearFromPosition(int row, int column){
	LCD_moveCursor(row,column);
	for(int i = column; i < 16; i++){
		LCD_writeChar(' ');
	}
}

void LCD_clr_1(){
	LCD_clearFromPosition(0,0);
}

void LCD_clr_2(){
	LCD_clearFromPosition(1,0);
}

void LCD_clr_xy(int x, int y){
	LCD_clearFromPosition(x,y);
}

void LCD_Startup(){
	LCD_DDR = 0xF0 | (1<<LCD_RS) | (1<<LCD_E);
	LCD_PORT = 0;

	LCD_PORT &= ~(1<<LCD_RS);	//Poinformowanie LCD ¿e nadchodz¹ce dane bêd¹ instrukcjami

	LCD_writeUpperBits(0b00110000);
	_delay_ms(5);
	LCD_writeUpperBits(0b00110000);
	_delay_ms(100);
	LCD_writeUpperBits(0b00110000);
	_delay_ms(100);
	LCD_writeUpperBits(0b00100000);
	_delay_ms(5);

	LCD_PORT |= 1<<LCD_RS;
	
	LCD_writeInstruction(0b00000110);
	LCD_writeInstruction(0b00001111);

	LCD_clear();
}
