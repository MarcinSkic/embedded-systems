#include <targets/AT91SAM7.h>
#include "PCF8833U8_lcd.h"

//Te nazwy odnosza sie do podlaczenia joysticka wedlug dokumentacji, na zajeciach inaczej
#define jUp 0x00000080;
#define jCenter 0x00000100;
#define jLeft 0x00000200;
#define jDown 0x00004000;
#define jRight 0x00008000;

enum menu{Home = 0b00000,Author = 0b00001,Figures=0b00010,Circle=0b10001,Square=0b10010,Triangle=0b10011,Trapeze=0b10100,Pictures=0b00011,Pic1=0b11001,Pic2=0b11010,TextAndPicture=0b00100};

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
      LCDPutStr("1.Autor",10,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("2.Figury",40,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("3.Obrazki",70,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("4.Tekst i obraz",100,5,MEDIUM,BLACK,WHITE);
      break;
    case Author:
      //DISPLAY
      LCDPutStr("Marcin Skic",10,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("13",30,5,MEDIUM,BLACK,WHITE);
      break;
    case Figures:
      submenusAmount = 4;
      LCDPutStr("1.Kolo",10,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("2.Kwadrat",40,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("3.Trojkat",70,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("4.Trapez",100,5,MEDIUM,BLACK,WHITE);
      break;
    case Circle:
      LCDSetCircle(65,65,50,RED);
      break;
    case Square:
      LCDSetRect(30,30,100,100,FILL,RED);
      break;
    case Triangle:
      LCDSetLine(5,5,125,5,RED);
      LCDSetLine(125,125,125,5,RED);
      LCDSetLine(125,125,5,5,RED);
      break;
    case Trapeze:
      LCDSetLine(5,20,125,5,RED);
      LCDSetLine(125,5,125,125,RED);
      LCDSetLine(125,125,5,110,RED);
      LCDSetLine(5,20,5,110,RED);
      break;
    case Pictures:
      submenusAmount = 2;
      LCDPutStr("1.Obraz",10,5,MEDIUM,BLACK,WHITE);
      LCDPutStr("2.Obraz",40,5,MEDIUM,BLACK,WHITE);
      break;
    case Pic1:
      LCDWrite130x130bmp(0);
      break;
    case Pic2:
      LCDWrite130x130bmp(1);
      break;
    case TextAndPicture:
      LCDWrite130x130bmp(1);
      LCDPutStr("Tekst",10,40,SMALL,BLACK,WHITE);
      LCDPutStr("i",20,40,SMALL,BLACK,WHITE);
      LCDPutStr("Obrazek",40,40,SMALL,BLACK,WHITE);
      break;
    
  }
  
  cursorPosition = 1;
  currentMenu = menu;

  if(submenusAmount != 0){
    LCDSetCircle(12+(cursorPosition-1)*textBarWidth,8,5,RED);
  }
}

void UpdateBarSelect(int previousCursorPosition){
  //LCDSetRect(0,textBarWidth*(cursorPosition-1),130,textBarWidth*cursorPosition,NOFILL,YELLOW);

  LCDSetCircle(12+(cursorPosition-1)*textBarWidth,8,5,RED);

  LCDSetCircle(12+(previousCursorPosition-1)*textBarWidth,8,5,WHITE);

  //LCDSetRect(0,textBarWidth*(previousCursorPosition-1),130,textBarWidth*previousCursorPosition,NOFILL,WHITE);
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

  if(cursorPosition >= submenusAmount+1){
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

  InitLCD();
  SetContrast(50);
  LCDClearScreen();

  //W³¹czenie guzików
  PIOB_PER |= 1<<24;
  PIOB_PER |= 1<<25;

  PIOB_ODR |= 1<<24;
  PIOB_ODR |= 1<<25;

  //W³¹czenie joysticków
  PMC_PCER = 1<<2;
  PIOA_PER |= jUp;
  PIOA_PER |= jCenter;
  PIOA_PER |= jDown;
  PIOA_PER |= jLeft
  PIOA_PER |= jRight;

  PIOA_ODR |= jUp;
  PIOA_ODR |= jCenter; 
  PIOA_ODR |= jDown; 
  PIOA_ODR |= jLeft; 
  PIOA_ODR |= jRight;
  
  loadMenu(Home);

  while(1){
    if((PIOA_PDSR & 1<<9) == 0){  //LEFT = 7//RIGHT = 14// UP = 9// DOWN = 15// Center = 8
      CursorUp();
      delay(500);
    }
    if((PIOA_PDSR & 1<<8) == 0){
      CursorDown();
      delay(500);
    }
    if((PIOA_PDSR & 1<<7) == 0 || ((PIOB_PDSR & 1<<24) == 0)){
      Back();
      delay(500);
    }
    if(((PIOA_PDSR & 1<<14) == 0) || ((PIOA_PDSR & 1<<15) == 0) || ((PIOB_PDSR & 1<<25) == 0)){
      Enter();
      delay(500);
    }
  }
}