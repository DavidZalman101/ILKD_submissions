//*********************************************
//                Utilities
//*********************************************

// Includes
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>

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

#define PERROR(msg)\
		fprintf(stderr, ANSI_COLOR_RED msg ANSI_COLOR_RESET)

#define PERROR_S(msg,s)\
		fprintf(stderr, ANSI_COLOR_RED msg ANSI_COLOR_RESET, s)
// ANSI color
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

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
char* subHomeDir_str(char*);
void subHomeDir_args(char**);
char* GetDirectionFile(char*,int*,char);
char* CleanCmdFromDirectionFiles(char*);
char **getPIPEDlines(char*);
char *getCleanExecline(char*);

// END
