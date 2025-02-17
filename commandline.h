//*********************************************
//                commandline.h
//*********************************************

// Includes
#include "utils.h"

//*********************************************
//*****************Cmd Object******************
//*********************************************

typedef struct commandline {
	char *line;
	char **pieces;
	char *in_file;
	char *out_file;
	int line_len;
	int pieces_num;
	int in_fd;
	int out_fd;
	int stdin_fd;
	int stdout_fd;
} Cmd;

typedef struct commandline *CmdPtr;

// Functions
void Cmdfree(CmdPtr);
void CmdfreePieces(CmdPtr);
CmdPtr Cmdalloc(char*);
int CmdWhatCmd(CmdPtr);
void CmdChangeDir(CmdPtr);
int CmdExec(CmdPtr);
int CmdRunChild(CmdPtr);
void* CmdFindInPath(CmdPtr);
int handleCMD(CmdPtr cptr);
int CmdRunPIPED(CmdPtr);
void RunChildRun(CmdPtr);

#define CMD_ERROR -1
#define CMD_NONE 0
#define CMD_EXIT 1
#define CMD_CD 2
#define CMD_EXEC 3
#define CMD_UNRECOGNIZED 4
#define CMD_RUN_CHILD 5
// END
