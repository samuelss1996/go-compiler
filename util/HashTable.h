#include "../Definitions.h"

typedef void* HashTable;

void createHashTable(HashTable* hashTable);
SymbolsTableValue findHash(HashTable* hashTable, char* key);
void insertHash(HashTable* hashTable, char* key, SymbolsTableValue value);
void destroyHashTable(HashTable* hashTable);