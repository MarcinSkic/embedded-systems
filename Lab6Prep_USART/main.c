#include <avr/io.h>

#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16))) - 1)	

void USART_Init(){
	UCSRB |= 1<<TXEN | 1<<RXEN;	//Uruchomienie transmisji i odbioru
	UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);	//Wybranie wielkosci znaku na 8 bit
	UBRRL = BAUD_PRESCALE;	//Ustawienie szybkosci przesylu (nizsze bity)
	UBRRH = BAUD_PRESCALE >> 8;	//Ustawienie szybkosci przesylu (wyzsze bity)
}

void USART_Transmit(char data){
	while(!(UCSRA & (1<<UDRE)));	//Dopoki bufor na dane nie zosta³ oprozniony (aka. dane nie zostaly wyslane) czekaj
	UDR = data;	//Zapisz dane do wyslania do bufora (co rowniez resetuje flage)
}

char USART_Receive(){
	while(!(UCSRA & (1<<RXC)));//Dopoki nie ma nieodczytanych danych (aka. nie otrzymalismy danych) czekaj
	
	return (UDR);	//Zwroc otrzymane dane	(odczytujemy dane co resetuje flage)
}

//START-Interruptions----------------
void USART_InitInterruptions(){
	UCSRB |= 1<<RXCIE;	//Wlacz przerwanie od flagi pojawienia sie nieodczytanych danych
	sei();	//Wlacz system przerwan
}

ISR(USART_RXC_vect){
	USART_Transmit(UDR);	//Odczytane dane wyslij przez USART
}
//END-Interruptions-----------------

void WaitingVersion(){
	USART_Init();

	while(1){
		USART_Transmit(USART_Receive());	//Odbierz dane i je wyslij przez USART
	}
}

void InterruptionsVersion(){
	USART_Init();
	USART_InitInterruptions();
	while(1);
}

int main(){
	InterruptionsVersion();
	
}
