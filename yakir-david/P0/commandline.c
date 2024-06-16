//*********************************************
//	  	 COMMAND LINE
//*********************************************

// INCLUDE
#include "commandline.h"

/*
 * getArgsSize
 *
 * @brief: 	This function returns the number of elements
 * 		inside a null-term array (not including the terminator)
 *
 * @param line:		char** args
 *
 * @return:		If the args is null terminated, it returns as wanter
 * 			O.w. anything can happen O_O
 *
 */
static int getArgsSize(char **args)
{
        if (args == NULL)
                return 0;
        int counter = 0;
        while (*args)
        {
                counter++;
                args++;
        }
        return counter;
}

/*
 * copysArg
 *
 * @brief: 	This function returns a complete copy of a nullterm arr
 *
 * @param line:		char** args
 *
 * @return:		On failure NULL, o.w. the copy
 *
 */
static char** copysArg(char **args, int idx, int n)
{
	if (args == NULL)
		return NULL;

        int pieces_num = getArgsSize(args);

        if (idx < 0 || n < 0 || idx + n > pieces_num)
                return NULL;

        int pieces_copy_num = n + 1;

        char **pieces_copy = NULL;
        MALLOC(pieces_copy,sizeof(char*) * pieces_copy_num);

        for (int i = 0; i < n; i++)
                MALLOC_STRCPY(pieces_copy[i],args[idx+i]);

        pieces_copy[pieces_copy_num-1] = NULL;
        return pieces_copy;
}

/*
 * freArgs
 *
 * @brief: 	This function frees a nullterm array
 *
 * @param line:		char** args
 *
 * @return:		Nothing
 *
 */
static void freeArgs(char **args)
{
	if (args == NULL)
		return;
	char **ptr = args;
	while (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
		ptr++;
	}
	free(args);
}

/*
 * getNumPieces
 *
 * @brief: 	This function returns the number of pieces
 * 		that exists in a given string
 *
 * @param line:		char*, the line given to the cmd
 *
 * @return:		On failure 0, o.w. the number of pieces.
 *
 */
static int getNumPieces(char *str)
{
	if (str == NULL)
		return 0;
	int count=0;
	while (*str)
	{
		if (isspace(*str)==0)
		{
			count++;
			while (*str && isspace(*str) == 0)
				str++;
		}
		else
		{
			while(*str && isspace(*str) !=0)
				str++;
		}
	}
	return count;
}

/*
 * getNumPieces
 *
 * @brief: 	This function returns a the NEXT piece in a given
 * 		string
 *
 * @param line:		char*, the line given to the cmd
 *
 * @return:		On failure NULL, o.w. the next piece.
 *
 */
static char* getPiece(char *str)
{
        if (str == NULL || strlen(str) == 1)
                return NULL;

        int i=0;
        while (isspace(str[i]) == 0)
                i++;
	char* piece = NULL;
	MALLOC(piece,sizeof(char) * (i+1));
        strncpy(piece,str,i);
        piece[i]='\0';
        return piece;
}

/*
 * getAllPieces
 *
 * @brief: 	This function retrun an array of string
 * 		which are all pieces from the given line.
 *
 * @param line:		char*, the line given to the cmd
 *
 * @return:		On failure OR if there are no pieces: NULL.
 * 			o.w. the wanted array.
 *
 */
static char** getAllPieces(char *line)
{
	if (line == NULL)
		return NULL;
	int n = getNumPieces(line), j = 0;
	size_t i = 0;
	if (n==0)
		return NULL;
	char **pieces;
	MALLOC(pieces,sizeof(char*) * (n + 1));
	while (i<strlen(line))
        {
                if (isspace(line[i])==0)
                {
                        char *piece = getPiece(line+i);
                        if (piece != NULL)
                        {
				pieces[j]=piece;
                                i += strlen(piece)-1;
				j++;
                        }
                }
                i++;
        }
	pieces[n] = NULL;
	return pieces;
}

/*
 * Cmdalloc
 *
 * @brief: 	This function allocated a Cmd
 *
 * @param line:		char*, the line given to the cmd
 *
 * @return:		On failure NULL, on success a pointer to the
 * 			newly constructed commandline.
 */
CmdPtr Cmdalloc(char *line)
{
	if (line == NULL)
		return NULL;

	int line_len = strlen(line);
	if (line_len <= 0)
		return NULL;

	// cptr
	CmdPtr cptr;
	MALLOC(cptr,sizeof(Cmd));

	cptr->line = NULL;
	cptr->pieces = NULL;
	cptr->line_len = 0;
	cptr->pieces_num = 0;

	// line
	MALLOC_STRCPY(cptr->line,line);
	cptr->line_len = line_len;

	// pieces
	cptr->pieces_num = getNumPieces(line);
	cptr->pieces = getAllPieces(line);

	return cptr;
}

/*
 * Cmdfree
 *
 * @brief: 	This function free's an allocared Cmd.
 *
 * @return:		Nothing.
 */
void CmdfreePieces(CmdPtr cptr)
{
	if (cptr == NULL || cptr->pieces == NULL)
		return;

	if (cptr->pieces != NULL)
	{
		assert(cptr->pieces_num != 0 && "cptr->pieces != NULL BUT cptr->pieces_num == 0");
		freeArgs(cptr->pieces);
		cptr->pieces = NULL;
		cptr->pieces_num = 0;
	}
	return;
}

/*
 * Cmdfree
 *
 * @brief: 	This function free's an allocared Cmd.
 *
 * @return:		Nothing.
 */
void Cmdfree(CmdPtr cptr)
{
	if (cptr == NULL)
		return;

	// free line
	if (cptr->line != NULL)
		free(cptr->line);

	cptr->line=NULL;
	cptr->line_len = 0;

	// free pieces
	CmdfreePieces(cptr);
	cptr->pieces=NULL;

	// final free
	free(cptr);
	return;
}

/*
 * CmdWhatCmd
 *
 * @brief: 	This function tells what is the command entered
 * 		Currently supporting exit/cd/exec
 *
 * @param cptr
 *
 * @return:		integer indicating what command was noticed or error.
 */
int CmdWhatCmd(CmdPtr cptr)
{
	if (cptr == NULL)
		return CMD_ERROR;

	if (cptr->pieces == NULL || cptr->pieces_num == 0)
		return CMD_NONE;

	if (strcmp(cptr->pieces[0], "exit") == 0)
		return cptr->pieces_num == 1 ? CMD_EXIT : CMD_ERROR; 	// [exit]

	if (strcmp(cptr->pieces[0], "cd") == 0)
		return CMD_CD;

	if (strcmp(cptr->pieces[0], "exec") == 0)
		return CMD_EXEC;

	if (cptr->pieces[0][0] == '.' || cptr->pieces[0][1] == '/')
		return CMD_RUN_CHILD;

	return CMD_UNRECOGNIZED;
}

/*
 * CmdChangeDir
 *
 * @brief: 	This function changes the cwd of the shell's proccess.
 *
 * @param CmdPtr
 *
 * @return:		integer indicating what command was noticed or error.
 *
 * @note:		On error, will print the relavent error msg w.r.t. error.
 * 			The msgs were taken from man 2 chdir.
 *
 */
void CmdChangeDir(CmdPtr cptr)
{
	if (cptr == NULL || cptr->pieces == NULL || cptr->pieces_num == 0)
		return;
	if (strcmp(cptr->pieces[0],"cd") != 0 )
		return;
	if (cptr->pieces_num != 2)
	{
		puts(ANSI_COLOR_RED "Error: one argument must be provided." ANSI_COLOR_RESET);
		puts(ANSI_COLOR_RED "Usage: cd <directory>" ANSI_COLOR_RESET);
		return;
	}
	if(chdir(cptr->pieces[1]) == -1)
		printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
	return;
}

/*
 * CmdCExec
 *
 * @brief: 	This function runs the exec with the approp args
 * 		If it fails, the shell will keep running, o.w. will die.
 *
 * @param CmdPtr
 *
 * @return:		Nothing.
 *
 */
void CmdExec(CmdPtr cptr)
{
	if (cptr == NULL || cptr->pieces == NULL || cptr->pieces_num == 0)
		return;
	if (strcmp(cptr->pieces[0],"exec") != 0)
		return;
	if (cptr->pieces_num <= 1)
	{
		puts(ANSI_COLOR_RED "Error: At least one argument must be provided." ANSI_COLOR_RESET);
		puts(ANSI_COLOR_RED "Usage: exec [arg0] [arg1] ... [argn]" ANSI_COLOR_RESET);
		return;
	}

	char **args = copysArg(cptr->pieces,1,cptr->pieces_num-1);

	if (execv(args[0],args) == -1)
		printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));

	freeArgs(args);
	args = NULL;
}
void CmdRunChild(CmdPtr cptr)
{

	if (cptr == NULL || cptr->pieces == NULL || cptr->pieces_num == 0)
		return;

	//The first piece of the input looks like a path (starts with . or /)
	if (cptr->pieces[0] == NULL || strlen(cptr->pieces[0]) == 0)
		return;
	if (cptr->pieces[0][0] != '.' && cptr->pieces[0][0] != '/')
		return;

	pid_t cpid;
	int status;

	cpid = fork();
	if (cpid == -1)
	{
		printf(ANSI_COLOR_RED "Error: Failed to fork PANIC!\n" ANSI_COLOR_RESET);
		return;
	}

	if (cpid == 0) /* Code executed by child */
	{
		if (execv(cptr->pieces[0],cptr->pieces) == -1)
			printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
		// Dont forget to exit the child process if fails
		exit(EXIT_FAILURE); 
	}
	else /* Code executed by parent */
	{
		// wait for the child proc to complete
		waitpid(cpid, &status, 0);
	}
}
