#include "LexicalComponent.h"
#include "../util/HashTable.h"
#include "../symbols/SymbolsTable.h"

typedef void* LexicalAnalyzer;

void createLexicalAnalyzer(LexicalAnalyzer *lexicalAnalyzer, char* file, SymbolsTable symbolsTable, HashTable operatorsTable);
LexicalComponent nextLexicalComponent(LexicalAnalyzer* lexicalAnalyzer);
void destroyLexicalAnalyzer(LexicalAnalyzer* lexicalAnalyzer);