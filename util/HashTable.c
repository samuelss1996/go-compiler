#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

typedef struct {
    LinkedList* elements;
    int elementsCount;
    int capacity;
} HashTableStruct;

typedef HashTableStruct* HashTable;

unsigned long calculateHash(char* key);

void createHashTable(HashTable* hashTable, int initialCapacity) {
    *hashTable = (HashTable) malloc(sizeof(HashTableStruct));

    (*hashTable)->elements = (LinkedList *) malloc(initialCapacity * sizeof(LinkedList));
    (*hashTable)->elementsCount = 0;
    (*hashTable)->capacity = initialCapacity;

    memset((*hashTable)->elements, 0, initialCapacity * sizeof(LinkedList));
}

SymbolsTableValue findHash(HashTable* hashTable, char* key) {
    unsigned long hash = calculateHash(key) % (*hashTable)->capacity;

    LinkedList possibilities = (*hashTable)->elements[hash];
    ListNode possibility = (possibilities != NULL)? firstNode(&possibilities) : NULL;
    ListItem item;

    while(possibility != NULL) {
        item = nodeItem(possibility);

        if(strcmp(itemKey(item), key) == 0) {
            return itemValue(item);
        }

        possibility = nextNode(possibility);
    }

    return TOKEN_NOT_FOUND;
}

void insertHash(HashTable* hashTable, char* key, SymbolsTableValue value) {
    unsigned long hash = calculateHash(key) % (*hashTable)->capacity;

    LinkedList targetList = (*hashTable)->elements[hash];
    ListItem newItem;

    if(targetList == NULL) {
        createList(&targetList);
        (*hashTable)->elements[hash] = targetList;
    }

    createItem(&newItem, key, value);
    append(&targetList, newItem);
}

void destroyHashTable(HashTable* hashTable) {
    for (int i = 0; i < (*hashTable)->capacity; ++i) {
        if((*hashTable)->elements[i] != NULL) {
            destroyList((*hashTable)->elements + i);
        }
    }

    free((*hashTable)->elements);
    free(*hashTable);
}

unsigned long calculateHash(char* key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++) != 0) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}