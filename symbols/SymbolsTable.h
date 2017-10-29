typedef void* SymbolsTable;

void createSymbolsTable(SymbolsTable* symbolsTable);
void addSymbol(SymbolsTable* symbolsTable, char *token, int id);
int findSymbol(SymbolsTable* symbolsTable, char *token);
void destroySymbolsTable(SymbolsTable* symbolsTable);