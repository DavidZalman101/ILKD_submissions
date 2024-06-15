//*********************************************
//	  	 COMMAND LINE
//*********************************************

// INCLUDE
#include "commandline.h"

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
	MALLOC(pieces,sizeof(char*) * n);
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
	MALLOC(cptr->line,sizeof(char)*(line_len+1));
	strcpy(cptr->line,line);
	(cptr->line)[line_len]='\0';
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

	if (cptr->pieces != NULL && cptr->pieces_num > 0)
	{
		// free each piece
		for (int i = 0; i < cptr->pieces_num; i++)
			if ( (cptr->pieces)[i] != NULL)
			{
				free((cptr->pieces)[i]);
				(cptr->pieces)[i] = NULL;
			}
		// free the arr of pieces
		free(cptr->pieces);
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

	// final free
	free(cptr);
	return;
}
