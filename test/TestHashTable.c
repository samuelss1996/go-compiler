#include "../util/HashTable.h"
#include "TestUtils.h"

void hashTableBasicTest() {
    HashTable test;

    createHashTable(&test, 10);
    insertHash(&test, "a", 1);
    insertHash(&test, "b", 2);
    insertHash(&test, "c", 3);
    insertHash(&test, "d", 4);
    insertHash(&test, "e", 5);
    insertHash(&test, "f", 6);

    assertEquals(findHash(&test, "a"), 1);
    assertEquals(findHash(&test, "b"), 2);
    assertEquals(findHash(&test, "c"), 3);
    assertEquals(findHash(&test, "d"), 4);
    assertEquals(findHash(&test, "e"), 5);
    assertEquals(findHash(&test, "f"), 6);

    destroyHashTable(&test);

    printf("[HashTable] Basic test: OK");
}