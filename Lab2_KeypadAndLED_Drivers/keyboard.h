#ifndef KEYBOARD_DRIVER
#define KEYBOARD_DRIVER

#include <avr/io.h>
#include <util/delay.h>

#define SW_DDR DDRB
#define SW_PORT PORTB
#define SW_PIN PINB

void SW_Conf();

unsigned int SW_GetKeyState(unsigned int SW_number);

unsigned int SW_Read();

#endif