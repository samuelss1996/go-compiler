#include "../input/InputSystem.h"
#include "LexicalComponent.h"
#include "../util/HashTable.h"

typedef void* LexicalAnalyzer;

void createLexicalAnalyzer(LexicalAnalyzer *lexicalAnalyzer, InputSystem inputSystem, SymbolsTable symbolsTable, HashTable operatorsTable);
LexicalComponent nextLexicalComponent(LexicalAnalyzer* lexicalAnalyzer);
void destroyLexicalAnalyzer(LexicalAnalyzer* lexicalAnalyzer);