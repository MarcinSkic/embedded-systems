#include "led.h"

void LED_Conf(unsigned int LED_number){
	LED_DDR |= 1<<LED_number;
}

void LED_Set(unsigned int LED_number) {
	LED_PORT |= 1<<LED_number;
}

void LED_Clear(unsigned int LED_number) {
	LED_PORT &= ~(1<<LED_number);
}

void LED_Toggle(unsigned int LED_number){
	LED_PORT ^= 1<<LED_number;
}

unsigned int LED_GetState(unsigned int LED_number){
	if(LED_PIN & (1<<LED_number)){
		return 1;
	} else {
		return 0;
	}
}