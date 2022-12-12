#include <stdio.h>

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
  //LCDClearScreen();
  submenusAmount = 0;

  switch(menu){
    case Home:
      submenusAmount = 4;
      //DISPLAY
      printf("Autor\n");
      printf("Figury\n");
      printf("Obrazki\n");
      printf("Tekst\n");
      break;
    case Author:
    printf("Marcin Skic\n");
      break;
    case Figures:
      submenusAmount = 4;
      printf("Menu Figur\n");
      break;
    case Circle:
      printf("Koło\n");
      break;
    case Square:
      printf("Kwadrat\n");
      break;
    case Triangle:
      printf("Trójkąt\n");
      break;
    case Trapeze:
      printf("Trapez\n");
      break;
    case Pictures:
      submenusAmount = 2;
      printf("Menu obrazków\n");
      break;
    case Pic1:
      printf("Obrazek 1\n");
      break;
    case Pic2:
      printf("Obrazek 2\n");
      break;
    case TextAndPicture:
      printf("Obrazek i tekst\n");
      break;

  }

  cursorPosition = 1;
  currentMenu = menu;
}

void UpdateBarSelect(int previousCursorPosition){
  //LCDSetRect(0,textBarWidth*cursorPosition,60,textBarWidth*cursorPosition+textBarWidth,NOFILL,YELLOW);
  printf("Pozycja kursora: %d\n",cursorPosition);
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

    char option;

  loadMenu(Home);
    Enter();
    getchar();
    Back();
    CursorDown();
    Enter();
    CursorUp();
    Enter();

  return 0;
}

