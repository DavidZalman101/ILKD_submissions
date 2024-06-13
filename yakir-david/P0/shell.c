#include "prompt.h"
#include <stdio.h>
#include <string.h>

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

		//puts("");
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
