#include "prompt.h"
#include <stdio.h>
#include <string.h>

/*
 * handleCMD
 *
 * @brief: 	This function handles the cmd line given by the user.
 * 		Is the user just hit ENTER, do nothing
 * 		If the user entered any text, print "Unrecognized command".
 *
 * @param p: 		char* line, the cmd line from the user.
 *
 * @return:		Nothing.
 */
void handleCMD(char *line)
{
	if (line == NULL || (strcmp(line,"\n") == 0))
		return;
	puts("Unrecognized command");
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
