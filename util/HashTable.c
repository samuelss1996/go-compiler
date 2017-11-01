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

/**
 * Crea una tabla hash. Se usa una tabla hash internamente en la tabla de símbolos, y de forma directa para la tabla de operadores
 * @param hashTable La tabla hash
 * @param initialCapacity La capacidad de la tabla hash (en caso de colisión se hace encadenamiento)
 */
void createHashTable(HashTable* hashTable, int initialCapacity) {
    *hashTable = (HashTable) malloc(sizeof(HashTableStruct));

    (*hashTable)->elements = (LinkedList *) malloc(initialCapacity * sizeof(LinkedList));
    (*hashTable)->elementsCount = 0;
    (*hashTable)->capacity = initialCapacity;

    memset((*hashTable)->elements, 0, initialCapacity * sizeof(LinkedList));
}

/**
 * Se busca un elemento, dado su clave
 * @param hashTable La tabla hash
 * @param key La clave
 * @return El valor asociado a la clave
 */
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

/**
 * Se inserta un nuevo elemento en la tabla hash, con su respectiva clave y su respectivo valor
 * @param hashTable La tabla hash
 * @param key La clave
 * @param value El valor
 */
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

/**
 * Destruir la tabla hash, liberando todos los recursos
 * @param hashTable La tabla hash
 */
void destroyHashTable(HashTable* hashTable) {
    for (int i = 0; i < (*hashTable)->capacity; ++i) {
        if((*hashTable)->elements[i] != NULL) {
            destroyList((*hashTable)->elements + i);
        }
    }

    free((*hashTable)->elements);
    free(*hashTable);
}

/**
 * Función de hash. Devuelve un entero dada una clave
 * @param key La clave
 * @return El entero calculado en base a la clave
 */
unsigned long calculateHash(char* key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++) != 0) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}