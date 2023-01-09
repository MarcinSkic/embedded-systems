#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int cursorX = 0, cursorY = 0;
int visuals[8][8];

enum fieldFlags {empty = 0,one = 1,isDiscovered = 0b0010000, isBomb = 0b0100000, isFlagged = 0b1000000};

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


void visualizeMap(){
    for (int i = 0; i < rows; i++){
        for(int x = 0; x < cols; x++){
            if(fields[i][x] & isBomb){
                printf("B");
            } else if(fields[i][x] > 0){
                printf("%d",(int)fields[i][x]);
            } else {
                printf("_");
            }
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));


    generateMap();
    visualizeMap();

    return 0;
}
