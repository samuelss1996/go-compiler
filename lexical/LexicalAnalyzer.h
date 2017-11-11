#include <stdio.h>
#include "LexicalComponent.h"
#include "../symbols/SymbolsTable.h"

typedef void* LexicalAnalyzer;

void createLexicalAnalyzer(LexicalAnalyzer *lexicalAnalyzer, FILE* file, SymbolsTable symbolsTable);
LexicalComponent nextLexicalComponent(LexicalAnalyzer* lexicalAnalyzer);
void destroyLexicalAnalyzer(LexicalAnalyzer* lexicalAnalyzer);