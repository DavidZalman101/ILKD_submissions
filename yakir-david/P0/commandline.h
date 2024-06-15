//*********************************************
//************CommandLine Object***************
//*********************************************

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>

typedef struct commandline {
	char *line;
	char **pieces;
	int line_len;
	int pieces_num;
} Cmd;

typedef struct commandline *CmdPtr;

// Functions
void Cmdfree(CmdPtr);
void CmdfreePieces(CmdPtr);
CmdPtr Cmdalloc(char*);

// MACROS
#define MALLOC(ptr,size) \
        do { \
                (ptr) = malloc((size)); \
                if ((ptr) == NULL) \
                        return NULL;\
        } while(0)

// END
