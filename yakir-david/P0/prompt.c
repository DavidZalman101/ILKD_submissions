//*********************************************
//	  	    PROMPT
//*********************************************

// INCLUDE
#include "prompt.h"


//*********************************************
//*************Implementations*****************
//*********************************************

/*
 * updatePromptCWD
 *
 * @brief: This function updated the cwd of a given prompt
 *
 * @param p: 		PromptPtr (prompt*) of the wanted prompt
 * 			for upadte.
 *
 * @return:		On success p, on failure NULL;
 *
 * @note:		This function should only be used by printPrompt.
 */
PromptPtr updatePromptCWD(PromptPtr p)
{
	if (p == NULL)
		return NULL;
	if (getcwd(p->cwd,sizeof(p->cwd)) == NULL)
		return NULL;
	return p;
}

/*
 * printPrompt
 *
 * @brief: 	This function prints the prompt, informing the user
 * 		about which absulute path they are currently in
 * 		followed by a $ and a space (e.g. /dir1/dir2$ ).
 *
 * @param p: 		PromptPtr (prompt*) of the prompt
 *
 * @return:		On failure will print nothing.
 */
void printPrompt(PromptPtr p)
{
	if (p == NULL)
		return;
	if (updatePromptCWD(p) != NULL)
	{
		printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET,p->cwd);
		printf("$ ");
	}
}

/*
 * Palloc
 *
 * @brief: 	This function allocated a prompt struct
 *
 * @return:		On failure NULL, on success a pointer to the
 * 			newly constructed prompt.
 */
PromptPtr Palloc(void)
{
	PromptPtr prompt_ptr = malloc (sizeof(Prompt));
	return prompt_ptr;
}

/*
 * Pfree
 *
 * @brief: 	This function free's an allocated Prompt.
 *
 * @return:		Nothing.
 */
void Pfree(PromptPtr p)
{
	if (p != NULL)
		free(p);
}

/*
 * printEntry
 *
 * @brief: 	This function prints a fun entry msg
 *
 * @return:		Nothing.
 *
 * @note:	Should only be called once at startup.
 */
void printEntry(void)
{
	puts(ANSI_COLOR_GREEN 	"*********************************************************************************");
	puts(		      	"* Welcome to our ****************************************************************");
	puts(		    	"*                              ____  _          _ _                             *");
	puts(			"*                             / ___|| |__   ___| | |				*");
	puts(			"*                             \\___ \\| '_ \\ / _ \\ | |                            *");
	puts(			"*                              ___) | | | |  __/ | |				*");
	puts(			"*                             |____/|_| |_|\\___|_|_|				*");
	puts(			"* By: yakir-david ***************************************************************");
	puts(			"*********************************************************************************" ANSI_COLOR_RESET);
}
