#include <stdio.h>
#include "../input/InputSystem.h"

// TODO finish test
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