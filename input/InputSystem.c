#include <stdlib.h>
#include "../Definitions.h"
#include "../errors/Errors.h"

typedef struct {
    FILE *file;
    char *blockA;
    char *blockB;
    char *frontPointer;
    char *backPointer;
    int currentBlock; // 0 o 1
    short validBlocks[2]; // flags que indican, para cada uno de los bloques, si se deben ser reemplazados o no
    int currentLine;
    int currentColumn;
    short newLineFlag; // flag auxiliar usado para conocer el número de línea actual
} InputSystemStruct;

typedef InputSystemStruct* InputSystem;

void moveBack(InputSystem* inputSystem, int positions);
int resetFrontPosition(InputSystem* inputSystem);
void destroyInputSystem(InputSystem* inputSystem);
void loadBlock(InputSystem* inputSystem, int block);
long frontAsLong(InputSystem* inputSystem);
void moveToOtherBlock(InputSystem* inputSystem);

/**
 * Crea e inicializa el sistema de entrada
 * @param inputSystem El sistema de entrada
 * @param filePath El fichero a compilar
 */
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

/**
 * Solicita el siguiente carácter al sistema de entrada
 * @param inputSystem El sistema de entrada
 * @return El carácter solicitado
 */
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

/**
 * Obtiene el último componente léxico leido como cadena de texto. En otras palabras, la secuencia de caracteres ubicada entre inicio y delantero
 * @param inputSystem El sistema de netrada
 * @param outBuffer El buffer en el que copiado el componente léxico. Se hará una reserva dinámica para ello
 */
void getReadToken(InputSystem* inputSystem, char **outBuffer) {
    int size, i;
    size = resetFrontPosition(inputSystem);

    *outBuffer = (char *) malloc((size + 1) * sizeof(char));

    for(i = 0; i < size; i++) {
        (*outBuffer)[i] = nextChar(inputSystem);
    }

    (*outBuffer)[i] = '\0';
}

/**
 * Retroceder una o varias posicones en el sistema de entrada, para volver a leer caracteres ya procesados
 * @param inputSystem El sistema de entrada
 * @param positions El número de posiciones a retroceder
 */
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

/**
 * Reiniciar el puntero delantero. En otras palabras, hacer que delantero = inicio
 * @param inputSystem El sistema de entrada
 * @return La distancia entre inicio y delantero antes de reiniciar
 */
int resetFrontPosition(InputSystem* inputSystem) {
    int i;
    for (i = 0; (*inputSystem)->frontPointer != (*inputSystem)->backPointer && i < BLOCK_SIZE_BYTES; i++) {
        moveBack(inputSystem, 1);
    }

    return i;
}

/**
 * Confirmar que se ha procesado el componente léxico. A efectos prácticos, mover inicio a la posición de delantero
 * @param inputSystem El sistema de entrada
 */
void confirmToken(InputSystem* inputSystem) {
    (*inputSystem)->backPointer = (*inputSystem)->frontPointer;
}

/**
 * Obtener la línea de fichero actual. En concreto, la línea correspondiente al carácter apuntado por delantero
 * @param inputSystem El sistema de entrada
 * @return El número de línea
 */
int getCurrentLine(InputSystem* inputSystem) {
    return (*inputSystem)->currentLine;
}

/**
 * Obtener la columna (número de carácter) del fichero actual. En concreto, la columna correspondiente al carácter apuntado por delantero
 * @param inputSystem El sistema de entrada
 * @return El número de columna
 */
int getCurrentColumn(InputSystem* inputSystem) {
    return (*inputSystem)->currentColumn;
}

/**
 * Destruir el sistema de entrada, liberando todos los recursos
 * @param inputSystem El sistema de entrada
 */
void destroyInputSystem(InputSystem* inputSystem) {
    free((*inputSystem)->blockA);
    free((*inputSystem)->blockB);

    if ((*inputSystem)->file != NULL) {
        fclose((*inputSystem)->file);
    }

    free(*inputSystem);
}

/**
 * Cargar un nuevo bloque leyendo el fichero
 * @param inputSystem El sistema de entrada
 * @param block El bloque sobre el que se debe cargar el fichero (0 = bloque a, 1 = bloque b)
 */
void loadBlock(InputSystem* inputSystem, int block) {
    char* blockToBeLoaded = (block == 0)? (*inputSystem)->blockA : (*inputSystem)->blockB;
    size_t readChars;

    if(!(*inputSystem)->validBlocks[block]) {
        readChars = fread(blockToBeLoaded, 1, BLOCK_SIZE_BYTES, (*inputSystem)->file);

        (*inputSystem)->validBlocks[block] = 1;
        blockToBeLoaded[readChars] = EOF;
    }
}

/**
 * Obtener la distancia, entre delantero y el comienzo de bloque, como un entero que indica la cantidad de caracteres
 * @param inputSystem El sistema de entrada
 * @return La distancia
 */
long frontAsLong(InputSystem* inputSystem) {
    char* currentBlockPointer = ((*inputSystem)->currentBlock == 0)? (*inputSystem)->blockA : (*inputSystem)->blockB;

    return (*inputSystem)->frontPointer - currentBlockPointer;
}

/**
 * Mover delantero al comienzo del bloque en el que NO se encuentra actualmente, y actualizar el resto de flags en consecuencia
 * @param inputSystem El sistema de entrada
 */
void moveToOtherBlock(InputSystem* inputSystem) {
    int currentBlockIndex = (*inputSystem)->currentBlock;

    (*inputSystem)->currentBlock = (currentBlockIndex == 0) ? 1 : 0;
    (*inputSystem)->frontPointer = (currentBlockIndex == 0) ? (*inputSystem)->blockB : (*inputSystem)->blockA;
}