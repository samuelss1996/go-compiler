typedef void* InputSystem;

void createInputSystem(InputSystem* inputSystem, char *filePath);
char nextChar(InputSystem* inputSystem);
void getReadToken(InputSystem* inputSystem, char *outBuffer);
void moveBack(InputSystem* inputSystem, int positions);
void confirmToken(InputSystem* inputSystem);
void destroyInputSystem(InputSystem* inputSystem);