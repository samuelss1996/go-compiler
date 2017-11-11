void unableToOpenFile(char* filename);
void unableToOpenDbFile(char* filename);

void expectingEndOfString(int line, int column);
void invalidEscapedCharInsideString(int line, int column, char *string);

void expectingEndOfRune(int line, int column);
void emptyRune(int line, int column);
void invalidEscapedCharInsideRune(int line, int column, char *rune);
void multiCharacterRune(int line, int column);

void invalidSymbol(int line, int column, char foundChar);