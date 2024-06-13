#include "prompt.h"
#include <stdio.h>
#include <string.h>

int main()
{
	PromptPtr p_ptr = Palloc();
	if (p_ptr == NULL)
		exit(EXIT_FAILURE);

	printPrompt(p_ptr);
	puts("");
	return 0;
}
