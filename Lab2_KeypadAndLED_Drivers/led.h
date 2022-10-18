#ifndef LED_DRIVER
#define LED_DRIVER

#define LED_DDR DDRA
#define LED_PORT PORTA
#define LED_PIN PINA

#include <avr/io.h>

void LED_Conf(unsigned int LED_number);

void LED_Set(unsigned int LED_number);

void LED_Clear(unsigned int LED_number);

void LED_Toggle(unsigned int LED_number);

unsigned int LED_GetState(unsigned int LED_number);

#endif
