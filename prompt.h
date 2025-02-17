//*********************************************
//		prompt.h
//*********************************************

// INCLUDES
#include "utils.h"

//*********************************************
//***************Prompt Object*****************
//*********************************************

typedef struct prompt {
	char cwd[PATH_MAX];
} Prompt;

typedef struct prompt *PromptPtr;

// Functions
void Pfree(PromptPtr);
PromptPtr Palloc(void);
PromptPtr updatePromptCWD(PromptPtr);
void printPrompt(PromptPtr);
void printEntry(void);

// END
