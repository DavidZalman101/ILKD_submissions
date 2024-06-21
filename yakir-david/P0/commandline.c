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
	cptr->in_file = NULL;
	cptr->out_file = NULL;
	cptr->line_len = 0;
	cptr->pieces_num = 0;
	cptr->in_fd=STDIN;
	cptr->out_fd=STDOUT;

	// line
	// Get redirection if exists
	int file_len = 0;
	char *file = GetDirectionFile(line,&file_len,'<');
	if (file != NULL && file_len == 0)
	{
		puts(ANSI_COLOR_RED"Error: Redirection with no file to redirect to!"ANSI_COLOR_RESET);
		goto ERROR_FREE_AND_EXIT_1;
	}
	//TODO: check strndup
	if (file != NULL)
		cptr->in_file = strndup(file,file_len);


	// Output aka >
	file = GetDirectionFile(line,&file_len,'>');
	if (file != NULL && file_len == 0)
	{
		puts(ANSI_COLOR_RED"Error: Redirection with no file to redirect to!"ANSI_COLOR_RESET);
		goto ERROR_FREE_AND_EXIT_2;
	}
	if (file != NULL)
		cptr->out_file = strndup(file,file_len);

	// Get a clean line of redirections
	cptr->line = CleanCmdFromDirectionFiles(line);
	if (cptr->line == NULL)
		goto ERROR_FREE_AND_EXIT_3;
	cptr->line_len = strlen(cptr->line);

	// pieces
	cptr->pieces_num = getNumPieces(cptr->line);
	cptr->pieces = getAllPieces(cptr->line);

	if (cptr->pieces == NULL)
		goto ERROR_FREE_AND_EXIT_5;

	// Save STDIN_FILENO and STDOUT_FILE_NO filedescriptors
	cptr->stdin_fd = dup(STDIN_FILENO);
	cptr->stdout_fd = dup(STDOUT_FILENO);

	if (cptr->stdin_fd == -1 || cptr->stdout_fd == -1)
		goto ERROR_FREE_AND_EXIT_4;

	return cptr;

	ERROR_FREE_AND_EXIT_1:
		free(cptr);
		return NULL;

	ERROR_FREE_AND_EXIT_2:
		free(cptr->in_file);
		free(cptr);
		return NULL;

	ERROR_FREE_AND_EXIT_3:
		free(cptr->in_file);
		free(cptr->out_file);
		free(cptr);
		return NULL;

	ERROR_FREE_AND_EXIT_4:
		printf(ANSI_COLOR_RED "Error: %s\n"\
				ANSI_COLOR_RESET, strerror(errno));

	ERROR_FREE_AND_EXIT_5:
		free(cptr->in_file);
		free(cptr->out_file);
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

	//// free in\out files
	if (cptr->in_file != NULL)
		free(cptr->in_file);
	if (cptr->out_file != NULL)
		free(cptr->out_file);

	// final free
	free(cptr);
	return;
}

/*
 * CmdRedirectOutput
 *
 * @brief: 	This function updateds the cmd output fd
 *
 * @param cptr: 	The cptr to update
 *
 * @return:		0 for success o.w. -1
 */
static int CmdRedirectOutput(CmdPtr cptr)
{
	if (cptr == NULL)
		return -1;

	if (cptr->out_file == NULL) /* No Redirection needed */
		return 0;

	/* Redirect stdout */
	if ((cptr->out_fd = open(cptr->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
		goto ERROR_CLOSE_AND_EXIT_0;

	if (dup2(cptr->out_fd, STDOUT_FILENO) == -1)
		goto ERROR_CLOSE_AND_EXIT_1;
	close(cptr->out_fd);
	return 0;

	ERROR_CLOSE_AND_EXIT_0:
		printf(ANSI_COLOR_RED "Error: Could not redirect output file %s\n"\
				ANSI_COLOR_RESET, cptr->in_file);
		return -1;

	ERROR_CLOSE_AND_EXIT_1:
		close(cptr->out_fd);
		printf(ANSI_COLOR_RED "Error: %s\n"\
				ANSI_COLOR_RESET, strerror(errno));
		return -1;
}

/*
 * CmdRedirectInput
 *
 * @brief: 	This function updateds the cmd input fd
 *
 * @param cptr: 	The cptr to update
 *
 * @return:		0 for success o.w. -1
 */
static int CmdRedirectInput(CmdPtr cptr)
{
	if (cptr == NULL)
		return -1;

	if (cptr->in_file == NULL) /* No Redirection needed */
		return 0;

	/* Redirect stdin */
	if ((cptr->in_fd = open(cptr->in_file, O_RDONLY)) == -1)
		goto ERROR_CLOSE_AND_EXIT_0;

	if (dup2(cptr->in_fd,STDIN_FILENO) == -1)
		goto ERROR_CLOSE_AND_EXIT_1;

	close(cptr->in_fd);
	return 0;

	ERROR_CLOSE_AND_EXIT_0:
		printf(ANSI_COLOR_RED "Error: Could not redirect input file %s\n"\
				ANSI_COLOR_RESET, cptr->in_file);
		return -1;

	ERROR_CLOSE_AND_EXIT_1:
		close(cptr->in_fd);
		printf(ANSI_COLOR_RED "Error: %s\n"\
				ANSI_COLOR_RESET, strerror(errno));
		return -1;
}

/*
 * CmdSetDefaultInOut 
 *
 * @brief: 	This function sets the fd for in\out of the 
 * 		shells proc back to STDIN_FILENO and STDOUT_FIKENO
 *
 * @param cptr
 *
 * @return:		Nothing
 */
static void CmdSetDefaultInOut(CmdPtr cptr)
{
	if (cptr == NULL)
		return;

	if (dup2(cptr->stdin_fd,STDIN_FILENO) == -1 || dup2(cptr->stdout_fd,STDOUT_FILENO) == -1)
		printf(ANSI_COLOR_RED "Error: %s\n"\
				ANSI_COLOR_RESET, strerror(errno));
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

	if (cptr->pieces == NULL || cptr->pieces_num == 0 || cptr->line_len == 0)
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

	execv(cptr->pieces[0],cptr->pieces);

	/* Redirect the cptr input and output if needed */
	if ((CmdRedirectInput(cptr) == -1) || (CmdRedirectOutput(cptr) == -1))
		goto EXIT;

	// Will return only if an error occured
	if (CmdFindInPath(cptr) == NULL)
		printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
	else
	{
		execv(cptr->pieces[0],cptr->pieces);
		printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
	}
	EXIT:
		// Must restore to the STDIN_FILENO and STROUT_FILENO
		CmdSetDefaultInOut(cptr);
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
		/* Redirect the cptr input and output if needed */
		if ((CmdRedirectInput(cptr) == -1) || (CmdRedirectOutput(cptr) == -1))
			goto EXIT;

		execv(cptr->pieces[0],cptr->pieces); /* Execute as is */

		if (CmdFindInPath(cptr) == NULL)
			printf(ANSI_COLOR_RED "Error: %s\n" ANSI_COLOR_RESET, strerror(errno));
		else
			execv(cptr->pieces[0],cptr->pieces); /* Execture from PATH */

		EXIT:
			// Dont forget to exit the child process if fails
			exit(EXIT_FAILURE); 
	}
	else /* Code executed by parent */
	{
		// wait for the child proc to complete
		waitpid(cpid, &status, 0);
	}
}
