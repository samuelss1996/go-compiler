#include <stdlib.h>
#include "../Definitions.h"

typedef struct {
    FILE *file;
    char *blockA;
    char *blockB;
    char *frontPointer;
    char *backPointer;
    int currentBlock;
    short validBlocks[2];
} InputSystemStruct;

typedef InputSystemStruct* InputSystem;

void loadBlock(InputSystem* inputSystem, int block);
long frontAsLong(InputSystem* inputSystem);
void moveToOtherBlock(InputSystem* inputSystem);
void moveBack(InputSystem* inputSystem, int positions);

void createInputSystem(InputSystem* inputSystem, char *filePath) {
    *inputSystem = (InputSystem) malloc(sizeof(InputSystemStruct));

    (*inputSystem)->file = fopen(filePath, "rb");
    (*inputSystem)->blockA = (char *) malloc(BLOCK_SIZE_BYTES + 1);
    (*inputSystem)->blockB = (char *) malloc(BLOCK_SIZE_BYTES + 1);
    (*inputSystem)->frontPointer = (*inputSystem)->backPointer = (*inputSystem)->blockA;
    (*inputSystem)->currentBlock = 0;
    (*inputSystem)->validBlocks[0] = (*inputSystem)->validBlocks[1] = 0;

    loadBlock(inputSystem, 0);
    loadBlock(inputSystem, 1);
}

char nextChar(InputSystem* inputSystem) {
    char result = *(*inputSystem)->frontPointer;

    (*inputSystem)->frontPointer++;

    if(*(*inputSystem)->frontPointer == EOF && frontAsLong(inputSystem) == BLOCK_SIZE_BYTES) {
        (*inputSystem)->validBlocks[(*inputSystem)->currentBlock] = 0;

        moveToOtherBlock(inputSystem);
        loadBlock(inputSystem, (*inputSystem)->currentBlock);
    }

    return result;
}

void getReadToken(InputSystem* inputSystem, char *outBuffer) {
    int size, i;

    for (size = 0; (*inputSystem)->frontPointer != (*inputSystem)->backPointer; size++) {
        moveBack(inputSystem, 1);
    }

    for(i = 0; i < size; i++) {
        outBuffer[i] = nextChar(inputSystem);
    }

    outBuffer[i] = '\0';
}

void moveBack(InputSystem* inputSystem, int positions) {
    long newIndex = frontAsLong(inputSystem) - positions;

    if(newIndex >= 0) {
        (*inputSystem)->frontPointer -= positions;
    } else {
        moveToOtherBlock(inputSystem);
        (*inputSystem)->frontPointer += BLOCK_SIZE_BYTES + newIndex;
    }
}

void confirmToken(InputSystem* inputSystem) {
    (*inputSystem)->backPointer = (*inputSystem)->frontPointer;
}

void destroyInputSystem(InputSystem* inputSystem) {
    free((*inputSystem)->blockA);
    free((*inputSystem)->blockB);

    fclose((*inputSystem)->file);

    free(*inputSystem);
}

void loadBlock(InputSystem* inputSystem, int block) {
    char* blockToBeLoaded = (block == 0)? (*inputSystem)->blockA : (*inputSystem)->blockB;
    size_t readChars;

    if(!(*inputSystem)->validBlocks[block]) {
        readChars = fread(blockToBeLoaded, sizeof(char), BLOCK_SIZE_BYTES, (*inputSystem)->file);

        (*inputSystem)->validBlocks[block] = 1;
        blockToBeLoaded[readChars] = EOF;
    }
}

long frontAsLong(InputSystem* inputSystem) {
    char* currentBlockPointer = ((*inputSystem)->currentBlock == 0)? (*inputSystem)->blockA : (*inputSystem)->blockB;

    return (*inputSystem)->frontPointer - currentBlockPointer;
}

void moveToOtherBlock(InputSystem* inputSystem) {
    int currentBlockIndex = (*inputSystem)->currentBlock;

    (*inputSystem)->currentBlock = (currentBlockIndex == 0) ? 1 : 0;
    (*inputSystem)->frontPointer = (currentBlockIndex == 0) ? (*inputSystem)->blockB : (*inputSystem)->blockA;
}