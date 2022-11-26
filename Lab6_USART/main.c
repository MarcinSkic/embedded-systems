#define F_CPU 8000000            /* Define frequency here its 8MHz */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (9600 * 16UL))) - 1)

void USART_Init(){
	UCSRB |= 1<<TXEN | 1<<RXEN;	//Uruchomienie transmisji i odbioru
	UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);	//Wybranie wielkosci znaku na 8 bit
	UBRRL = BAUD_PRESCALE;	//Ustawienie szybkosci przesylu (nizsze bity)
	UBRRH = BAUD_PRESCALE >> 8;	//Ustawienie szybkosci przesylu (wyzsze bity)
}

void USART_Transmit(char data){
	while(!(UCSRA & (1<<UDRE)));	//Dopoki bufor na dane nie zosta? oprozniony (aka. dane nie zostaly wyslane) czekaj
	UDR = data;	//Zapisz dane do wyslania do bufora (co rowniez resetuje flage)
}

void USART_SendString(char *word){
	unsigned char i = 0;
	while(word[i] != 0){
		USART_Transmit(word[i]);
		i++;
	}
}

unsigned char USART_Receive(){
	while(!(UCSRA & (1<<RXC)));	//Dopoki nie ma nieodczytanych danych (aka. nie otrzymalismy danych) czekaj
	return (UDR);	//Zwroc otrzymane dane	(odczytujemy dane co resetuje flage)
}

void USART_InitInterruptions(){
	UCSRB |= 1<<RXCIE;	//Wlacz przerwanie od flagi pojawienia sie nieodczytanych danych
	sei();	//Wlacz system przerwan
}

void WaitingVersion(){
	USART_Init();
	char c;

	while(1){
		c = USART_Receive();
		USART_Transmit(c);	//Odbierz dane i je wyslij przez USART
	}
}

void InterruptionsVersion(){
	USART_Init();
	USART_InitInterruptions();
	while(1);
}

void ex1(){
	USART_Init();
	
	USART_SendString("Marcin Skic");
}

void ex2(){
	USART_Init();
	srand(time(NULL));   
	
	while(1){
		int randomKey = rand()%24+97; 
		
		USART_SendString("Nacisnij klawisz [");
		USART_Transmit(randomKey);
		USART_SendString("]: ");
		
		char c = USART_Receive();
		USART_SendString("\r\n");
		if(c == randomKey){
			USART_SendString("Prawidlowy znak");
		} else {
			USART_SendString("Bledny znak");
		}
		USART_SendString("\r\n");
	}
}

volatile int countCycles = 0;

ISR(TIMER0_COMP_vect){
	countCycles++;
	if(countCycles >= 40){ //Obliczone dla taktowania 8MHz -> zmiana stanu co sekunde
		PORTA ^= 1;
		countCycles = 0;
	}
}

ISR(USART_RXC_vect){
	char command = UDR;
	USART_Transmit(command);
	
	if(command > 48 && command < 56){
		PORTA ^= 1<<(command-48);
	} else if(command == 'c'){
		PORTA &= 0b00000001;
	} else if(command == 's'){
		PORTA |= 0b11111110;
	} else {
		USART_SendString("\r\n");
		USART_SendString("Bledne polecenie");
	}
	
	USART_SendString("\r\n");
	USART_SendString("Oczekiwanie na polecenie: ");
}
//Skic Marcin
void ex3(){
	USART_Init();
	USART_InitInterruptions();
	
	DDRA = 0xff;	//Diody

	//---------------------TIMER-------------------------
	TCCR0 |= (1 << CS00) | (1 << CS02);
    TCCR0 |= (1 << WGM01);
	TIMSK |= (1<<OCIE0);

    TCNT0 = 0;
    OCR0  = 196;	//Obliczone dla taktowania 8MHz -> zmiana stanu co sekunde
	//---------------------/TIMER-------------------------
	
	USART_SendString("Oczekiwanie na polecenie: ");
	while(1);
}

int main(){
	ex1();
	//ex2();
	//ex3();
}
