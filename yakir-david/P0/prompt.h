//*********************************************
//		prompt.h
//*********************************************

// INCLUDES
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

// ANSI color
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
