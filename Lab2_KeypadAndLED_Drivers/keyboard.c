#include "keyboard.h"

void SW_Conf() {
	SW_DDR = 0xF0;
	SW_PORT = 0x0F;
}

unsigned int SW_GetKeyState(unsigned int SW_number){
	int column = SW_number%4+4;
	int row = SW_number/4;
	int x;
	
	SW_PORT = ~(1<<column);
	
	_delay_ms(1);
	
	x = SW_PIN & 0x0F;
	
	if(x == (0x0F&(~(1<<row)))){
		return 1;
	} else {
		return 0;
	}
	
}

unsigned int SW_Read(){
	while(1){
		for(int i = 0; i < 16; i++){
			if(SW_GetKeyState(i)){
				return i;
			}
		}
	}
}