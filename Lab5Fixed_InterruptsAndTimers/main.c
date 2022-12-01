/*
 * Lab5.c
 *
 * Created: 14.11.2022 14:07:15
 * Author : Student_PL
 */ 
#define F_CPU 1000000 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define KEYPAD_PORT PORTA
#define KEYPAD_PIN PINA

volatile uint8_t waitTime = 10;

int getKey(){
	
	int x, result = 0;
	for(int i = 4; i < 8; i++){
		KEYPAD_PORT = ~(1<<i);
		_delay_ms(1);
		
		x = KEYPAD_PIN & 0x0F;
		_delay_ms(5);
		
		if(x != (KEYPAD_PIN & 0x0F)) break;
		
		switch(x){
			case 0b00001111:
			break;
			case 0b00001110:			
				result = 1+i-4;
				break;
			case 0b00001101:
				result = 5+i-4;
				break;
			case 0b00001011:
				result = 9+i-4;
				break;
			case 0b00000111:
				result = 13+i-4;
				break;
			default:
			if(x!=0b00001111) result = 0xFF;
		}
	}
	return result;
}

void wait100ms(){
	while(!(TIFR & (1<<OCF0)));
	TIFR |= OCF0;
}

void wait1sec(){
	for(int i = 0; i < 10; i++){
		wait100ms();
	}
}

void waitBasedOnButton(){
	for(int i = 0; i < waitTime; i++){
		wait100ms();
	}
}

ISR (TIMER0_COMP_vect){
	cli();
	
	wait1sec();
	PORTC ^= 0x01;
	
	sei();
	
}

ISR (INT0_vect){	
	unsigned int klawisz = getKey();
	
	if(klawisz < 15) {
		waitTime = klawisz;
	} else if (klawisz == 15){
		waitTime = 0;
	} else if (klawisz == 16){
		waitTime = 1;
	}
	
	GIFR |= 1 <<INTF0;
	PORTA = 0x0F;
}



void ex1and2(){
	DDRC |= 0x01;
	PORTC = 0;
	
	TCCR0 |= (1 << CS00) | (1 << CS02);	//Prescaler 1024
	TCCR0 |= 1<<WGM01;	//Tryb CTC
	OCR0 = 98;	//Wartosc do porownania przez CTC
	
	TIMSK |= 1<<OCIE0;	//Zezwolenie na aktywowanie przerwania przez CTC0
	
	sei();	//Wlaczenie systemu przerwan
	
	while(1);
}

void ex3(){
	//Diody
	DDRC = 0xff;
	PORTC = 0;
	
	//Klawiatura
	DDRA = 0xF0;
	PORTA = 0x0F;
	
	//Przerwanie INT0
	DDRD = 0x00;	
	//PORTD = 0xFF;
	
	TCCR0 |= (1 << CS00) | (1 << CS02);	//Prescaler 1024
	TCCR0 |= 1<<WGM01;	//Tryb CTC
	OCR0 = 98;	//Wartosc do porownania przez CTC
	
	GIMSK |= 1<<INT0;	//W³¹czenie przerwania na INT0
	MCUCR |= 1<<ISC01; //Ustawienie reakcji przerwania INT0 na negacjê stanu
	
	sei();
	
	while(1){
		waitBasedOnButton();
		if(waitTime != 0){
			PORTC ^= 0x01;
		} else {
			PORTC = 0;
		}
	}
}

int main(void)
{
   //ex1and2();
	ex3();
}

