#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int cursorX = 0, cursorY = 0;

enum fieldFlags {isDiscovered = 0b001, isBomb = 0b010, isFlagged = 0b100};

int minesNumber = 10;
int rows = 8;
int cols = 8;

enum fieldFlags fields[8][8];   //rows, cols
int visuals[8][8];

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
    }
}

void visualizeMap(){
    for (int i = 0; i < rows; i++){
        for(int x = 0; x < cols; x++){
            if(fields[i][x] & isBomb){
                printf("B");
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
