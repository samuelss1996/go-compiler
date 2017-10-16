#include "InputSystem.h"
#include "../Definitions.h"

#include <stdlib.h>
#include <stdio.h>

FILE *file;

char blockA[BLOCK_SIZE_BYTES + 1];
char blockB[BLOCK_SIZE_BYTES + 1];
int currentBlock = 0;
int currentIndex = 0;

void loadBlock(int block);

void init(char* filePath) {
    file = fopen(filePath, "rb");

    blockA[BLOCK_SIZE_BYTES] = blockB[BLOCK_SIZE_BYTES] = EOF;

    loadBlock(0);
}

char nextChar() {
    char result;

    if(currentBlock == 0) {
        result = blockA[currentIndex];
    } else {
        result = blockB[currentIndex];
    }

    if(result == EOF) {
        if(currentIndex == BLOCK_SIZE_BYTES) {
            currentBlock = (currentBlock == 0)? 1 : 0;
            currentIndex = 0;

            loadBlock(currentBlock);
            result = nextChar();
        }
    } else {
        currentIndex++;
    }

    return result;
}

void sendBack() {

}

void destroy() {
    fclose(file);
}

void loadBlock(int block) {
    switch(block) {
        case 0:
            fread(blockA, 1, BLOCK_SIZE_BYTES, file);
            break;
        default:
            fread(blockB, 1, BLOCK_SIZE_BYTES, file);
            break;
    }
}