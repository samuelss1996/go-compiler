#include <stdio.h>
#include <stdlib.h>

/** Función util para los tests, aborta si no se cumple la condición de igualdad */
void assertEquals(int actual, int expected) {
    if(actual != expected) {
        printf("Assertion Error: Expecting %d. Got %d", expected, actual);
        exit(-1);
    }
}