#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int cursorX = 0, cursorY = 0;
int visuals[8][8];

enum fieldFlags {empty = 0,one = 1,isDiscovered = 0b0010000, isBomb = 0b0100000, isFlagged = 0b1000000};
enum gameState {pending,won,gameOver};

enum gameState gameState = pending;
int minesNumber = 10;
int rows = 8;
int cols = 8;

enum fieldFlags fields[8][8];   //rows, cols

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
        gameState = gameOver;
        return;
    }

    if(field & isBomb || gameState == gameOver){
        gameState = gameOver;
        return;
    }

    if(!(field & isDiscovered) /*&& !(field & isFlagged)*/){    //This condition should be verified in user input part of code, on higher level
        fields[row][col] |= isDiscovered;

        if((field & ~(isFlagged)) == 0) {    //It should take flagged fields in mass discover
            if (row != 0) {
                discoverField(row - 1, col);   //TOP

                if (col != 0) {
                    discoverField(row - 1, col - 1);   //TOP LEFT
                }
                if (col < cols - 1) {
                    discoverField(row - 1, col + 1);   //TOP RIGHT
                }
            }

            if (col != 0) {
                discoverField(row, col - 1);   //LEFT
            }
            if (col < cols - 1) {
                discoverField(row, col + 1);   //RIGHT
            }

            if (row < rows - 1) {
                discoverField(row + 1, col);   //BOTTOM

                if (col != 0) {
                    discoverField(row + 1, col - 1); //BOTTOM LEFT
                }
                if (col < cols - 1) {
                    discoverField(row + 1, col + 1); //BOTTOM RIGHT
                }
            }
        }

        fields[row][col] &= ~(isFlagged);
    }
}

void flagField(int row, int col){
    if(!(fields[row][col] & isDiscovered)){
        if(fields[row][col] & isFlagged){
            fields[row][col] &= ~(isFlagged);
        } else {
            fields[row][col] |= isFlagged;
        }
    }
}

void showFieldSymbol(int i, int x, int ignoreFlags) {
    if (i == cursorY && x == cursorX) {
        printf("\033[0;31m");
    }

    if ((fields[i][x] & isDiscovered) || ignoreFlags) {

        if (fields[i][x] & isBomb) {
            printf("B");
        } else if ((fields[i][x] & 0b1111) > 0) {   //0b1111 numbers field can have, from 0 to 8
            printf("%d", (int) (fields[i][x] & 0b1111));
        } else {
            printf("-");
        }
    } else if(fields[i][x] & isFlagged) {
        printf("F");
    } else {
        printf("_");
    }

    if(i == cursorY && x == cursorX){
        printf("\033[0m");
    }
}

void visualizeMap(){
    for (int i = 0; i < rows; i++){
        for(int x = 0; x < cols; x++){
            showFieldSymbol(i,x,0);
        }
        printf("\n");
    }

    //CODE FOR NORMAL CONSOLES, DOESN'T WORK IN CLION
    /*for(int i = 0; i < rows; i++){
        printf("\033[A");
    }
    printf("\r");*/

    printf("\n");
}

void visualizeDiscoveredMap(){
    for (int i = 0; i < rows; i++){
        for(int x = 0; x < cols; x++){
            showFieldSymbol(i,x,1);
        }
        printf("\n");
    }

    /*for(int i = 0; i < rows; i++){
        printf("\033[A");
    }
    printf("\r");*/

    printf("\n\n\n");
}

int main() {
    srand(time(NULL));

    while(1){

        generateMap();
        visualizeDiscoveredMap();

        while(gameState == pending){
            visualizeMap();
            char choice = fgetc(stdin);

            switch (choice) {
                case 'w':
                    if(cursorY != 0){
                        cursorY--;
                    }
                    break;
                case 's':
                    if(cursorY < rows-1){
                        cursorY++;
                    }
                    break;
                case 'a':
                    if(cursorX != 0){
                        cursorX--;
                    }
                    break;
                case 'd':
                    if(cursorX < cols-1) {
                        cursorX++;
                    }
                    break;
                case 'e':
                    if(!(fields[cursorY][cursorX] & isFlagged)) {
                        discoverField(cursorY, cursorX);
                    }
                    break;
                case 'q':
                    flagField(cursorY,cursorX);
                    break;
            }

            fflush(stdin);
            visualizeDiscoveredMap();
        }

        if(gameState & won){
            printf("You WON!\n");
        } else {
            printf("You LOST!");
        }

        printf("Try again? [y/N]: ");
        char choice = fgetc(stdin);
        choice = fgetc(stdin);

        if(choice == 'y'){
            gameState = pending;
            cursorX = 0;
            cursorY = 0;
        } else {
            return 0;
        }
    }

    return 0;
}
