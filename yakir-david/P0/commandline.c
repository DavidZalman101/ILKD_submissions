//*********************************************
//	  	 COMMAND LINE
//*********************************************

// INCLUDE
#include "commandline.h"

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
	cptr->line = strdup(line);
	if (cptr->line == NULL)
		goto ERROR_FREE_AND_EXIT_1;

	cptr->line_len = line_len;

	// pieces
	cptr->pieces_num = getNumPieces(line);
	cptr->pieces = getAllPieces(line);

	if (cptr->pieces == NULL)
		goto ERROR_FREE_AND_EXIT_2;

	return cptr;

	ERROR_FREE_AND_EXIT_1:
		free(cptr);
		return NULL;

	ERROR_FREE_AND_EXIT_2:
		free(cptr->line);
		free(cptr);
		return NULL;
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

// TODO: complete descrp
void *CmdFindInPath(CmdPtr cptr)
{
	if (cptr == NULL)
		return NULL;


	char *file_in_PATH = FindInPath(cptr->pieces[0]);
	if (file_in_PATH == NULL)
		return NULL;
	free(cptr->pieces[0]);
	cptr->pieces[0] = file_in_PATH;

	return cptr;
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
	CmdfreePieces(cptr);
	cptr->pieces = args;
	cptr->pieces_num--;

	//execv(args[0],args);
	execv(cptr->pieces[0],cptr->pieces);

	// Will return only if an error occured
	if (CmdFindInPath(cptr) == NULL)
		printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
	else
	{
		execv(cptr->pieces[0],cptr->pieces);
		printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
	}

	//freeArgs(args);
	//args = NULL;
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
		execv(cptr->pieces[0],cptr->pieces);
		if (CmdFindInPath(cptr) == NULL)
			printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
		else
			execv(cptr->pieces[0],cptr->pieces);

		// Dont forget to exit the child process if fails
		exit(EXIT_FAILURE); 
	}
	else /* Code executed by parent */
	{
		// wait for the child proc to complete
		waitpid(cpid, &status, 0);
	}
}
