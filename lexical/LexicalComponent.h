typedef void* LexicalComponent;

void createLexicalComponent(LexicalComponent* lexicalComponent, int componentId, char* componentToken);
int getLexicalComponentId(LexicalComponent* lexicalComponent);
char* getLexicalComponentToken(LexicalComponent* lexicalComponent);
void destroyLexicalComponent(LexicalComponent* lexicalComponent);