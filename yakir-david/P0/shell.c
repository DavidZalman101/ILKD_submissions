#include "utils.h"
#include "prompt.h"
#include "commandline.h"
#include <stdio.h>
#include <string.h>

/*
 * handleCMD
 *
 * @brief: 	This function handles the cmd line given by the user.
 * 		Is the user just hit ENTER, do nothing
 * 		Instead of just printing "Unrecognized command" 
 * 		include the name of the program in the error message.
 * 		e.g. cat shell.c -> "Unrecognized command: cat".
 *
 * @param p: 		char* line, the cmd line from the user.
 *
 * @return:		Nothing.
 */
int handleCMD(CmdPtr cptr)
{
	int ret = CMD_NONE;
	char *og_cmd = strdup(cptr->pieces[0]);

	if (cptr == NULL || cptr->pieces_num == 0)
		return CMD_NONE;

	switch (CmdWhatCmd(cptr))
	{
		case CMD_EXIT:
			ret = CMD_EXIT;
			break;

		case CMD_ERROR:
			printf("error\n");
			ret=CMD_EXIT;
			break;

		case CMD_CD:
			CmdChangeDir(cptr);
			break;

		case CMD_EXEC:
			CmdExec(cptr);
			break;

		case CMD_RUN_CHILD:
		      CmdRunChild(cptr);
		      break;

		case CMD_NONE:
			break;

		case CMD_UNRECOGNIZED:
			// Try running from PATH
			if (CmdFindInPath(cptr) != NULL)
				CmdRunChild(cptr);
			else
				printf(ANSI_COLOR_RED "Unrecognized command: %s\n" ANSI_COLOR_RESET, og_cmd);
			break;
		default:
			break;
	}
	free(og_cmd);
	return ret;
}

int main()
{
	printEntry();
	Prompt p;
	updatePromptCWD(&p);
	CmdPtr cptr = NULL;

	char *line = NULL;
	size_t len = 0;
	ssize_t nread = 0;

	printPrompt(&p);

	while ((nread = getline(&line,&len,stdin)) != -1)
	{
		if (strcmp(line,"^D") == 0)
		       goto FREE_ALL;

		cptr = Cmdalloc(line);
		if (cptr == NULL)
		{
			puts("Error: Couldn't allocate Commandline struct!");
			goto FREE_ALL;
		}

		free(line);
		line = NULL;

		if (handleCMD(cptr) == CMD_EXIT)
			break;

		printPrompt(&p);

		Cmdfree(cptr);
		cptr = NULL;
	}

	FREE_ALL:
		if (line != NULL)
			free(line);
		if (cptr != NULL)
			Cmdfree(cptr);
	puts("");
	return 0;
}
