#include <stdlib.h>
#include "../Definitions.h"
#include "../errors/Errors.h"

typedef struct {
    FILE *file;
    char *blockA;
    char *blockB;
    char *frontPointer;
    char *backPointer;
    int currentBlock;
    short validBlocks[2];
    int currentLine;
    int currentColumn;
    short newLineFlag;
} InputSystemStruct;

typedef InputSystemStruct* InputSystem;

void moveBack(InputSystem* inputSystem, int positions);
int resetFrontPosition(InputSystem* inputSystem);
void destroyInputSystem(InputSystem* inputSystem);
void loadBlock(InputSystem* inputSystem, int block);
long frontAsLong(InputSystem* inputSystem);
void moveToOtherBlock(InputSystem* inputSystem);

void createInputSystem(InputSystem* inputSystem, char *filePath) {
    *inputSystem = (InputSystem) malloc(sizeof(InputSystemStruct));

    (*inputSystem)->file = fopen(filePath, "rb");
    (*inputSystem)->blockA = (char *) malloc(BLOCK_SIZE_BYTES + 1);
    (*inputSystem)->blockB = (char *) malloc(BLOCK_SIZE_BYTES + 1);
    (*inputSystem)->frontPointer = (*inputSystem)->backPointer = (*inputSystem)->blockA;
    (*inputSystem)->currentBlock = 0;
    (*inputSystem)->validBlocks[0] = (*inputSystem)->validBlocks[1] = 0;
    (*inputSystem)->currentLine = 1;
    (*inputSystem)->currentColumn = 0;
    (*inputSystem)->newLineFlag = 0;

    if((*inputSystem)->file == NULL) {
        unableToOpenFile(filePath);
        destroyInputSystem(inputSystem);

        exit(EXIT_CODE_UNABLE_TO_OPEN_FILE);
    }

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

    if((*inputSystem)->newLineFlag > 0) {
        (*inputSystem)->currentLine++;
        (*inputSystem)->currentColumn = 0;
        (*inputSystem)->newLineFlag = 0;
    }

    if(result == '\n') {
        (*inputSystem)->newLineFlag++;
    }

    (*inputSystem)->currentColumn++;

    return result;
}

void getReadToken(InputSystem* inputSystem, char **outBuffer) {
    int size, i;
    size = resetFrontPosition(inputSystem);

    *outBuffer = (char *) malloc((size + 1) * sizeof(char));

    for(i = 0; i < size; i++) {
        (*outBuffer)[i] = nextChar(inputSystem);
    }

    (*outBuffer)[i] = '\0';
}

void moveBack(InputSystem* inputSystem, int positions) {
    long newIndex = frontAsLong(inputSystem) - positions;

    (*inputSystem)->currentColumn -= positions;

    if(newIndex >= 0) {
        (*inputSystem)->frontPointer -= positions;
    } else {
        moveToOtherBlock(inputSystem);
        (*inputSystem)->frontPointer += BLOCK_SIZE_BYTES + newIndex;
    }

    if (*(*inputSystem)->frontPointer == '\n') {
        (*inputSystem)->newLineFlag--;
    }
}

int resetFrontPosition(InputSystem* inputSystem) {
    int i;
    for (i = 0; (*inputSystem)->frontPointer != (*inputSystem)->backPointer && i < BLOCK_SIZE_BYTES; i++) {
        moveBack(inputSystem, 1);
    }

    return i;
}

void confirmToken(InputSystem* inputSystem) {
    (*inputSystem)->backPointer = (*inputSystem)->frontPointer;
}

int getCurrentLine(InputSystem* inputSystem) {
    return (*inputSystem)->currentLine;
}

int getCurrentColumn(InputSystem* inputSystem) {
    return (*inputSystem)->currentColumn;
}

void destroyInputSystem(InputSystem* inputSystem) {
    free((*inputSystem)->blockA);
    free((*inputSystem)->blockB);

    if ((*inputSystem)->file != NULL) {
        fclose((*inputSystem)->file);
    }

    free(*inputSystem);
}

void loadBlock(InputSystem* inputSystem, int block) {
    char* blockToBeLoaded = (block == 0)? (*inputSystem)->blockA : (*inputSystem)->blockB;
    size_t readChars;

    if(!(*inputSystem)->validBlocks[block]) {
        readChars = fread(blockToBeLoaded, 1, BLOCK_SIZE_BYTES, (*inputSystem)->file);

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