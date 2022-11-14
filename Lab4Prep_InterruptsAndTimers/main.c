/*
 * Lab6_LCD_2.c
 *
 * Created: 2022-06-15 08:06:38
 * Author : Student
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

//TASK 1
ISR(INT0_vect) {
	PORTA ^= 1 << PA0;
}

ISR(INT2_vect) {
	PORTA ^= 1 << PA2;
}

void task1(){
	DDRA = 1 << PA0 | 1 << PA2;
	MCUCR &= ~(1 << ISC00);
	MCUCR |= 1 << ISC01;
	MCUCSR &= ~(1 << ISC2);
	GICR |= 1 << INT0 | 1 << INT2;
	GIFR |= 1 << INT0 | 1 << INT2;
	sei();
}

volatile char countCycles;

//TASK2
ISR(TIMER0_COMP_vect){
	countCycles++;
	PORTA = countCycles;
}

void task2(){
	DDRA = (1 << 0) | (1 << 1);
	TCCR0 |= (1 << CS00) | (1 << CS02);
    TCCR0 |= (1 << WGM01);
	TIMSK |= (1<<OCIE0);

    TCNT0 = 0;
    OCR0  = 98;

	sei();
	
}

int main(void) {
	
	task2();
	while(1) {}
}
