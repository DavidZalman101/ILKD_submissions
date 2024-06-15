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
void handleCMD(char *line)
{
	if (line == NULL || (strcmp(line,"\n") == 0) || strlen(line) == 0)
		return;

	CmdPtr cptr = Cmdalloc(line);
	if (cptr == NULL || cptr->pieces_num == 0)
		goto FREE_AND_EXIT;

	printf("Unrecognized command: %s\n", cptr->pieces[0]);

	FREE_AND_EXIT:
		if (cptr != NULL)
			Cmdfree(cptr);
		return;
}

int main()
{
	printEntry();
	PromptPtr p_ptr = Palloc();
	if (p_ptr == NULL)
		exit(EXIT_FAILURE);

	char *line = NULL;
	size_t len = 0;
	ssize_t nread = 0;

	printPrompt(p_ptr);
	while ((nread = getline(&line,&len,stdin)) != -1)
	{
		if (strcmp(line,"^D") == 0)
			break;

		handleCMD(line);
		printPrompt(p_ptr);
	}

	goto FREE_ALL;
	FREE_ALL:
		if (line != NULL)
			free(line);
		if (p_ptr != NULL)
			Pfree(p_ptr);
	puts("");
	return 0;
}
