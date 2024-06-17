//*********************************************
//****************Utilities********************
//*********************************************

// Includes
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

// MACROS
#define MALLOC(ptr,size)\
        do { \
                (ptr) = malloc((size)); \
                if ((ptr) == NULL)\
                        return NULL;\
        } while(0)

#define STRDUP(dest,source)\
        do {\
               (dest) = strdup((source));\
                if (dest == NULL)\
                        return NULL;\
        } while(0)


// FUNCTIONS
int getArgsSize(char**);
char** copysArg(char**,int,int);
void freeArgs(char**);
int getPATHNumDirs();
char **getPATHDirs();
char** appendToAllPaths(char**, char*);
int getNumPieces(char*);
char* getPiece(char*);
char** getAllPieces(char*);
void* FindInPath(char*);

// END
