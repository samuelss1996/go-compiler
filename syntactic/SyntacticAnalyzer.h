#include "../lexical/LexicalAnalyzer.h"

typedef void* SyntacticAnalyzer;

void createSyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer, LexicalAnalyzer lexicalAnalyzer);
void startSyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer);
void destroySyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer);