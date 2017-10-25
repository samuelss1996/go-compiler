#include <stdlib.h>

#define HASH_TABLE_INITIAL_SIZE 2000

typedef char* HashTableKey;
typedef int HashTableValue;

typedef struct {
    HashTableValue* values;
    int elementsCount;
} HashTableStruct;

typedef HashTableStruct* HashTable;
// TODO implement hash table expansion


void createHashTable(HashTable* hashTable) {
    *hashTable = (HashTable) malloc(sizeof(HashTableStruct));

    (*hashTable)->elementsCount = 0;
    (*hashTable)->values = (HashTableValue *) malloc(HASH_TABLE_INITIAL_SIZE * sizeof(HashTableValue));
}

HashTableValue findHash(HashTable* hashTable, HashTableKey key) {

}

void insertHash(HashTable* hashTable, HashTableKey key, HashTableValue value) {

}

void destroyHashTable(HashTable* hashTable) {
    // TODO free
    // TODO free
}

int calculateHash(HashTableKey key) {

}