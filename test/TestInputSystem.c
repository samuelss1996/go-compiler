#include <stdio.h>
#include "../input/InputSystem.h"

/** Test básico para el sistema de entrada */
void inputSystemBasicTest() {
    InputSystem inputSystem;
    char current;

    createInputSystem(&inputSystem, "../concurrentSum.go");
    current = nextChar(&inputSystem);

    while(current != EOF) {
        printf("%c", current);
        current = nextChar(&inputSystem);
    }
}