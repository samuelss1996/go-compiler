#include "InputSystem.h"
#include "../Definitions.h"

FILE *file;

char blockA[BLOCK_SIZE_BYTES + 1];
char blockB[BLOCK_SIZE_BYTES + 1];

int startBlock = 0;
int startIndex = 0;
int frontBlock = 0;
int frontIndex = 0;

void loadBlock(int block);

// TODO proper handling of goBack between blocks when going forward again
void initInputSystem(char *filePath) {
    file = fopen(filePath, "rb");

    blockA[BLOCK_SIZE_BYTES] = blockB[BLOCK_SIZE_BYTES] = EOF;

    loadBlock(0);
}

char nextChar() {
    char result;

    if(frontBlock == 0) {
        result = blockA[frontIndex];
    } else {
        result = blockB[frontIndex];
    }

    if(result == EOF) {
        if(frontIndex == BLOCK_SIZE_BYTES) {
            frontBlock = (frontBlock == 0)? 1 : 0;
            frontIndex = 0;

            loadBlock(frontBlock);
            result = nextChar();
        }
    } else {
        frontIndex++;
    }

    return result;
}

char* getReadToken(char *outBuffer) {
    int backupBlock = frontBlock;
    int backupIndex = frontIndex;
    int i;

    frontBlock = startBlock;
    frontIndex = startIndex;

    for (i = 0; (frontBlock != backupBlock || frontIndex != backupIndex); ++i) {
        outBuffer[i] = nextChar();
    }

    outBuffer[i] = '\0';

    return outBuffer;
}

// TODO consider out of range move
void moveBack(int positions) {
    frontIndex -= positions;

    if(frontIndex < 0) {
        frontBlock = (frontBlock == 0)? 1 : 0;
        frontIndex = BLOCK_SIZE_BYTES - frontIndex;
    }
}

void moveForward() {
    startBlock = frontBlock;
    startIndex = frontIndex;
}

void destroyInputSystem() {
    fclose(file);
}

void loadBlock(int block) {
    size_t readChars;

    switch(block) {
        case 0:
            readChars = fread(blockA, sizeof(char), BLOCK_SIZE_BYTES, file);
            blockA[readChars] = EOF;
            break;
        default:
            readChars = fread(blockB, sizeof(char), BLOCK_SIZE_BYTES, file);
            blockA[readChars] = EOF;
            break;
    }


}