#include <stdio.h>
#include <stdlib.h>
#include <targets/AT91SAM7.h>
#include "PCF8833U8_lcd.h"

#define jLeft 0x00000080;
#define jDown 0x00000100;
#define jUp 0x00000200;
#define jRight 0x00004000;
#define jCenter 0x00008000;

int cursorX = 0, cursorY = 0;
int visuals[8][8];

enum fieldFlags {empty = 0,one = 1,isDiscovered = 0b0010000, isBomb = 0b0100000, isFlagged = 0b1000000};
enum gameState {pending,won,gameOver};

enum gameState gameState = pending;
int minesNumber = 10;
int rows = 8;
int cols = 8;

enum fieldFlags fields[8][8];   //rows, cols

void delay(int ms){
    for(int i = 0; i < ms; i++){
        for(int x = 0; x < 3000; x++){
            __asm__("NOP");
        }
    }
}

void generateNumbers(int row,int col){
    if(row != 0){
        fields[row-1][col]++;   //TOP

        if(col != 0){
            fields[row-1][col-1]++; //TOP LEFT
        }
        if(col < cols-1){
            fields[row-1][col+1]++; //TOP RIGHT
        }
    }

    if(col != 0){
        fields[row][col-1]++;   //LEFT
    }
    if(col < cols-1){
        fields[row][col+1]++;   //RIGHT
    }

    if(row < rows-1){
        fields[row+1][col]++;   //BOTTOM

        if(col != 0){
            fields[row+1][col-1]++; //BOTTOM LEFT
        }
        if(col < cols-1){
            fields[row+1][col+1]++; //BOTTOM RIGHT
        }
    }

}

void generateMap(){

    int decidedMines = 0;
    int selectedMines [minesNumber];
    while(decidedMines < minesNumber){
        int pos = rand()%(rows*cols);
        int posTaken = 0;

        for(int i = 0; i < decidedMines; i++){
            if(selectedMines[i] == pos){
                posTaken = 1;
                break;
            }
        }

        if(!posTaken) {
            selectedMines[decidedMines] = pos;
            decidedMines++;
        }
    }

    for (int i = 0; i < rows; i++){
        for(int x = 0; x < cols; x++){
            fields[i][x] = 0;
        }
    }

    for(int i = 0; i < minesNumber; i++){
        int minePos = selectedMines[i];
        fields[minePos/cols][minePos%cols] = isBomb;
        generateNumbers(minePos/cols,minePos%cols);
    }
}

void discoverField(int row, int col){
    enum fieldFlags field = fields[row][col];

    if(gameState == gameOver){
        return;
    } else if(field & isBomb){    

        fields[row][col] |= isDiscovered;
        LCDSetRect(row*16+2,col*16+2,(row+1)*16,(col+1)*16,FILL,WHITE);
        showFieldSymbol(row,col,0);

        gameState = gameOver;
        return;
    }

    if(!(field & isDiscovered)){
        
        fields[row][col] |= isDiscovered;
    
        if((field & ~(isFlagged)) == 0){    //It should take flagged fields in mass discover
            if(row != 0){
                discoverField(row-1,col);   //TOP
                if(col != 0){
                    discoverField(row-1,col-1);   //TOP LEFT
                }
                if(col < cols-1){
                    discoverField(row-1,col+1);   //TOP RIGHT
                }
            }
            if(col != 0){
                discoverField(row,col-1);   //LEFT
            }
            if(col < cols-1){
                discoverField(row,col+1);   //RIGHT
            }
            if(row < rows-1){
                discoverField(row+1,col);   //BOTTOM
                if(col != 0){
                    discoverField(row+1,col-1); //BOTTOM LEFT
                }
                if(col < cols-1){
                    discoverField(row+1,col+1); //BOTTOM RIGHT
                }
            }
        }

        fields[row][col] &= ~(isFlagged);
        LCDSetRect(row*16+2,col*16+2,(row+1)*16,(col+1)*16,FILL,WHITE);
        showFieldSymbol(row,col,0);
    }
}

void showFieldSymbol(int i, int x,int ignoreFlags){
  if((fields[i][x] & isDiscovered) || ignoreFlags) {
      if (fields[i][x] & isBomb) {
          LCDPutChar('B',i*16+2,x*16+2,LARGE,BLACK,WHITE);
      } else if(fields[i][x] & isFlagged & !ignoreFlags){
          LCDPutChar('F',i*16+2,x*16+2,LARGE,BLACK,WHITE);
      } else if ((fields[i][x] & 0b1111) > 0) {
          LCDPutChar((int) (fields[i][x] & 0b1111) + 48,i*16+2,x*16+2,LARGE,BLACK,WHITE);
      } else {
          LCDPutChar('-',i*16+2,x*16+2,LARGE,BLACK,WHITE);
      }
  } else {
      LCDSetRect(i*16+2,x*16+2,(i+1)*16,(x+1)*16,FILL,WHITE);
  }
}

void visualizeMap(){
    LCDClearScreen();

    for (int i = 0; i < rows; i++){
        for(int x = 0; x < cols; x++){
            showFieldSymbol(i,x,0);
        }
    }
}

void flagField(int row, int col){
  if(!(fields[row][col] & isDiscovered)){
    if(fields[row][col] & isFlagged){
      LCDSetRect(row*16+2,col*16+2,(row+1)*16,(col+1)*16,FILL,WHITE);
      fields[row][col] &= ~(isFlagged);
    } else {
      LCDSetRect(row*16+2,col*16+2,(row+1)*16,(col+1)*16,FILL,WHITE);
      LCDPutChar('F',row*16+2,col*16+2,LARGE,BLACK,WHITE);
      fields[row][col] |= isFlagged;
    }
  }
}

void visualizeDiscoveredMap(){
    LCDClearScreen();

    for (int i = 0; i < rows; i++){
        for(int x = 0; x < cols; x++){
              showFieldSymbol(i,x,1);
        }
    }
}

void clearCursor(){
   LCDSetRect(cursorY*16+1,cursorX*16+1,(cursorY+1)*16+1,(cursorX+1)*16+1,NOFILL,WHITE);
}

void generateCursor(){
  LCDSetRect(cursorY*16+1,cursorX*16+1,(cursorY+1)*16+1,(cursorX+1)*16+1,NOFILL,RED);
}

int main() {

    PMC_PCER = 1<<3;
    PIOB_PER = 1<<20;
    PIOB_OER = 1<<20;

    InitLCD();
    SetContrast(30);
    LCDClearScreen();

    PIOB_PER |= 1<<24;
    PIOB_PER |= 1<<25;

    PIOB_ODR |= 1<<24;
    PIOB_ODR |= 1<<25;

    //Wlaczenie joysticków
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

    while(1){
      srand(1); //time(NULL) zawsze zwraca to samo, nie ma sensu uzywac
    
      generateMap();
      visualizeMap();
      generateCursor();

      while(gameState == pending){
          if((PIOA_PDSR & 1<<9) == 0){
            if(cursorY != 0){
              clearCursor();
              cursorY--;
              generateCursor();
            }
            delay(500);
          }
          if((PIOA_PDSR & 1<<8) == 0){
            if(cursorY < rows-1){
              clearCursor();
              cursorY++;
              generateCursor();
            }
            delay(500);
          }
          if((PIOA_PDSR & 1<<7) == 0){
          
            if(cursorX != 0){
              clearCursor();
              cursorX--;
              generateCursor();
            }
            delay(500);
          }
          if(((PIOA_PDSR & 1<<14) == 0) || ((PIOA_PDSR & 1<<15) == 0)){
            if(cursorX < cols-1) {
              clearCursor();
              cursorX++;
              generateCursor();
            }
            delay(500);
          }
          if((PIOB_PDSR & 1<<25) == 0){
            if(!(fields[cursorY][cursorX] & isFlagged)) {
              discoverField(cursorY, cursorX);
            }
            delay(500);
          }

          if((PIOB_PDSR & 1<<24) == 0){
            flagField(cursorY,cursorX);
            delay(500);
          }
      }

      if(gameState & won){
          clearCursor();
          visualizeDiscoveredMap();

          LCDPutStr("You WON!",50,50,LARGE,BLACK,WHITE);
      } else {
          clearCursor();
          visualizeDiscoveredMap();

          LCDPutStr("You LOST!",50,50,LARGE,BLACK,WHITE);
      }

      while(1){
        if((PIOB_PDSR & 1<<25) == 0){
          gameState = pending;
          cursorX = 0;
          cursorY = 0;
          break;  //Kontynuowanie gry
        }
        if((PIOB_PDSR & 1<<24) == 0){
          LCDClearScreen();
          return; //Zakonczenie
        }
      }
    }
    return 0;
}
