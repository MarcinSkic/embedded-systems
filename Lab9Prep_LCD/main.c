#include <targets/AT91SAM7.h>
#include "PCF8833U8_lcd.h"
#include "bmp.h"

#define jUp 0x00000080;
#define jCenter 0x00000100;
#define jLeft 0x00000200;
#define jDown 0x00004000;
#define jRight 0x00008000;

enum menu{Home = 0b00000,Author = 0b00001,Figures=0b00010,Circle=0b10000,Square=0b10001,Triangle=0b10010,Trapeze=0b10011,Pictures=0b00011,Pic1=0b11000,Pic2=0b11001,TextAndPicture=0b00100};

enum menu currentMenu = Home;
int cursorPosition = 1;
int submenusAmount;
int textBarWidth = 30;

void delay(int ms){
    for(int i = 0; i < ms; i++){
        for(int x = 0; x < 3000; x++){
            __asm__("NOP");
        }
    }
}

void loadMenu(enum menu menu){
  LCDClearScreen();
  submenusAmount = 0;

  switch(menu){
    case Home:
      submenusAmount = 4;
      //DISPLAY
      LCDPutStr("Autor",0,30,MEDIUM,BLACK,WHITE);
      LCDPutStr("Figury",0,60,MEDIUM,BLACK,WHITE);
      LCDPutStr("Obrazki",0,90,MEDIUM,BLACK,WHITE);
      LCDPutStr("Tekst z obrazkiem",0,120,MEDIUM,BLACK,WHITE);
      break;
    case Author:
      //DISPLAY
      break;
    case Figures:
      submenusAmount = 4;
      //DISPLAY
      break;
    case Circle:
      break;
    case Square:
      break;
    case Triangle:
      break;
    case Trapeze:
      break;
    case Pictures:
      submenusAmount = 2;
      break;
    case Pic1:
      break;
    case Pic2:
      break;
    case TextAndPicture:
      break;
    
  }
  
  cursorPosition = 1;
  currentMenu = menu;
}

void UpdateBarSelect(int previousCursorPosition){
  LCDSetRect(0,textBarWidth*(cursorPosition-1),130,textBarWidth*cursorPosition,NOFILL,YELLOW);

  LCDSetRect(0,textBarWidth*(previousCursorPosition-1),130,textBarWidth*previousCursorPosition,NOFILL,WHITE);
}

void CursorUp(){
  if(submenusAmount == 0) return;

  int previousCursorPosition = cursorPosition;
  cursorPosition--;

  if(cursorPosition < 1){
    cursorPosition = submenusAmount;
  }

  UpdateBarSelect(previousCursorPosition);
}

void CursorDown(){
  if(submenusAmount == 0) return;

  int previousCursorPosition = cursorPosition;
  cursorPosition++;

  if(cursorPosition+1 == submenusAmount){
    cursorPosition = 1;
  }

  UpdateBarSelect(previousCursorPosition);
}


void Enter(){
  if(submenusAmount == 0) return;

  loadMenu((currentMenu << 3)+cursorPosition);
}

void Back(){
    if(currentMenu == Home) return;

  loadMenu(currentMenu >> 3);
}

int main(){
  //Wlaczenie podswietlenia LCD
  PMC_PCER = 1<<3;
  PIOB_PER = 1<<20;
  PIOB_OER = 1<<20;

  //Włączenie joysticków

  
  InitLCD();
  SetContrast(30);
  LCDClearScreen();
  
  
}