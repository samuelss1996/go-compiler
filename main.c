#include <stdio.h>
#include "input/InputSystem.h"

int main() {
    int i = 0;
    init("../concurrentSum.go");

    for(i = 0; i < 100; i++) {
        printf("%c", nextChar());
    }

    printf("\n");
}