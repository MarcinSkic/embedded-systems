#include <targets/AT91SAM7.h>
#include "PCF8833U8_lcd.h"

enum menu{Home = 0b00000,Author = 0b00011,Figures=0b00001,Circle=0b01000,Square=0b01001,Triangle=0b01010,Trapeze=0b01011,Pictures=0b00010,Pic1=0b10000,Pic2=0b10001,TextAndPicture=0b00100};

enum menu currentMenu = Home;
int cursorPosition = 0;

void loadMenu(enum menu menu){
  
  switch(menu){
    case Home:
      //LCDPutStr("Autor",0,0);
      LCDPutStr("Autor",0,0,LARGE,BLACK,WHITE);
      LCDPutStr("Figury",0,30,LARGE,BLACK,WHITE);
      break;
  }
  if(menu < 0b00011){
    cursorPosition = 0; 
  } else {
    cursorPosition = -1;
  }
  
  currentMenu = menu;
}

int main(){
  //Wlaczenie podswietlenia LCD
  PMC_PCER = 1<<3;
  PIOB_PER = 1<<20;
  PIOB_OER = 1<<20;
  PIOB_SODR = 1<<20;
  
  InitLCD();
  SetContrast(30);
  LCDClearScreen();
  
  
}