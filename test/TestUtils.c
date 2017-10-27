#include <stdio.h>
#include <stdlib.h>

void assertEquals(int actual, int expected) {
    if(actual != expected) {
        printf("Assertion Error: Expecting %d. Got %d", expected, actual);
        exit(-1);
    }
}