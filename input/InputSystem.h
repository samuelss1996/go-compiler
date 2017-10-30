typedef void* InputSystem;

void createInputSystem(InputSystem* inputSystem, char *filePath);
char nextChar(InputSystem* inputSystem);
void getReadToken(InputSystem* inputSystem, char *outBuffer);
void moveBack(InputSystem* inputSystem, int positions);
int resetFrontPosition(InputSystem* inputSystem);
void confirmToken(InputSystem* inputSystem);
int getCurrentLine(InputSystem* inputSystem);
int getCurrentColumn(InputSystem* inputSystem);
void destroyInputSystem(InputSystem* inputSystem);