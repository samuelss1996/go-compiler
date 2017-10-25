typedef void* HashTable;
typedef char* HashTableKey;
typedef int HashTableValue;

void createHashTable(HashTable* hashTable);
HashTableValue findHash(HashTable* hashTable, HashTableKey key);
void insertHash(HashTable* hashTable, HashTableKey key, HashTableValue value);
void destroyHashTable(HashTable* hashTable);