#include "utils.h"
#include "prompt.h"
#include "commandline.h"
#include <stdio.h>
#include <string.h>

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
		if (strcmp(line,"\n") == 0)
			goto CONTINUE;

		if (strcmp(line,"^D") == 0)
		       goto FREE_ALL;

		cptr = Cmdalloc(line);
		free(line);
		line = NULL;
		if (cptr == NULL)
			goto CONTINUE;


		if (handleCMD(cptr) == EXIT_FAILURE)
			break;


		Cmdfree(cptr);
		cptr = NULL;
		CONTINUE:
			printPrompt(&p);
	}

	FREE_ALL:
		if (line != NULL)
			free(line);
		if (cptr != NULL)
			Cmdfree(cptr);
	puts("");
	return 0;
}
