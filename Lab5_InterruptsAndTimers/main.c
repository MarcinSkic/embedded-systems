/*
 * Lab5.c
 *
 * Created: 14.11.2022 14:07:15
 * Author : Student_PL
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define KEYPAD_PORT PORTA
#define KEYPAD_PIN PINA

volatile int timerMax = 10;
volatile int timeCounter = 0;

volatile int cycles = 0;


void temp(){
	cycles++;
	if(cycles >= 10){
		cycles = 0;
		PORTC ^= 0x01;
	}
	TCNT0 = 157;
	TIFR |= 1<<TOV0;
}

ISR (TIMER0_OVF_vect){
	timeCounter++;
	if(timeCounter >= timerMax){
		timeCounter = 0;
		PORTC ^= 0x01;
	}
}

ISR (TIMER0_COMP_vect){
	
}

void wait_100ms(){
	while(!(TIFR & (1<<TOV0)));
	TCNT0 = 157;

	TIFR |= 1<<TOV0;
}

void wait_1sec(){
	for(int i = 0; i < 10; i++){
		wait_100ms();
	}
	PORTC ^= 0x01;
}

void ex1(){
	DDRC |= 0x01;

	TCCR0 |= (1 <<CS02) | (1 << CS00);
	TCNT0 = 157;

	while(1){
		wait_1sec();
	}
}



void ex2(){
	DDRC |= 0x01;

	TCCR0 |= (1 << CS02) | (1 << CS00);
	TCNT0 = 157;

	TIMSK |= 1<<TOIE0;
	sei();

	while(1){
		asm("nop");
	}
}

int getKey(){
	//Zabezpieczenie przed stykaniem / Kod bledu
	
	int x, result = 0, buttonsClicked = 0;
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

ISR (INT0_vect){
	int klawisz = getKey();
	
	if(klawisz < 15) {
		timerMax = klawisz;
	} else if (klawisz == 15){
		TIMSK &= ~(1<<OCIE0);
	} else if (klawisz == 16){
		TIMSK |= 1<<OCIE0;
	}
	
	PORTA = 0x0F;
}

int main(void)
{
	
	//Diody
	DDRC = 0xff;
	PORTC = 0;
	
	//Klawiatura
	DDRA = 0xF0;
	PORTA = 0x0F;
	
	//Przerwanie INT0
	DDRD = 0x00;	
	PORTD = 0xFF;
	
	TCCR0 |= (1 << CS00) | (1 << CS02);	//Prescaler 1024
	TCNT0 = 157;
	
	TIMSK |= 1<<TOIE0;	//Zezwolenie na aktywowanie przerwania przez CTC0
	
	GIMSK |= 1<<INT0;	//W��czenie przerwania na INT0
	MCUCR |= 1<<ISC01 | 1<<ISC00; //Ustawienie reakcji przerwania INT0 na negacj� stanu
	
	sei();	//Wlaczenie systemu przerwan
	
    while(1){
		
		//ZAPISUJ TO DO VOLATILE
		int klawisz = getKey();

		if(klawisz != 0){
			if(klawisz < 15) {
				timerMax = klawisz;
			} else if (klawisz == 15){
				TIMSK &= ~(1<<TOIE0);
			} else if (klawisz == 16){
				TIMSK |= 1<<TOIE0;
			}
	
			PORTA = 0x0F;
		}
	
		
	}
    //ex1();
	//ex2();
}
