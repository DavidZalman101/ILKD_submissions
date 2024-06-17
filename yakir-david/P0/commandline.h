//*********************************************
//************CommandLine Object***************
//*********************************************

// Includes
#include "utils.h"

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
int CmdWhatCmd(CmdPtr);
void CmdChangeDir(CmdPtr);
void CmdExec(CmdPtr);
void CmdRunChild(CmdPtr);
void* CmdFindInPath(CmdPtr);

#define CMD_ERROR -1
#define CMD_NONE 0
#define CMD_EXIT 1
#define CMD_CD 2
#define CMD_EXEC 3
#define CMD_UNRECOGNIZED 4
#define CMD_RUN_CHILD 5

// ANSI color
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// END
