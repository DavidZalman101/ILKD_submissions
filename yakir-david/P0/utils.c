//*********************************************
//	  	    UTILITIES 
//*********************************************

#include "utils.h"



/*
 * getArgsSize
 *
 * @brief: 	This function returns the number of elements
 * 		inside a null-term array (not including the terminator)
 *
 * @param line:		char** args
 *
 * @return:		If the args is null terminated, it returns as wanter
 * 			O.w. anything can happen O_O
 *
 */
int getArgsSize(char **args)
{
        if (args == NULL)
                return 0;
        int counter = 0;
        while (*args)
        {
                counter++;
                args++;
        }
        return counter;
}

/*
 * copysArg
 *
 * @brief: 	This function returns a complete copy of a nullterm arr
 *		but flexiple of what to copy
 *
 * @param line:		char** args
 *
 * @return:		On failure NULL, o.w. the copy
 *
 */
char** copysArg(char **args, int idx, int n)
{
	if (args == NULL)
		return NULL;

        int pieces_num = getArgsSize(args);

        if (idx < 0 || n < 0 || idx + n > pieces_num)
                return NULL;

        int pieces_copy_num = n + 1;

        char **pieces_copy = NULL;
        MALLOC(pieces_copy,sizeof(char*) * pieces_copy_num);

        for (int i = 0; i < n; i++)
		STRDUP(pieces_copy[i],args[idx+i]);

        pieces_copy[pieces_copy_num-1] = NULL;
        return pieces_copy;
}
/*
 * freArgs
 *
 * @brief: 	This function frees a nullterm array
 *
 * @param line:		char** args
 *
 * @return:		Nothing
 *
 */
void freeArgs(char **args)
{
	if (args == NULL)
		return;
	char **ptr = args;
	while (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
		ptr++;
	}
	free(args);
}

/*
 * getPATHNumDirs
 *
 * @brief: 	This function retruns the number
 * 		of dirs in the PATH var
 *
 * @return:		int
 *
 */
int getPATHNumDirs()
{
	char *PATH = getenv("PATH");
	int n = 0;
	while (*PATH)
	{
		if (*PATH == ':')
			n++;
		PATH++;
	}
	n++;
	return n;

}

/*
 * getPATHDirs
 *
 * @brief: 	This function retruns all the dirs
 * 		in the PATH var
 *
 * @return:		char**, arr of strings
 *
 */
char **getPATHDirs()
{
	char *PATH = getenv("PATH");
        char *PATHC = NULL;

        if (PATH == NULL)
                return NULL;

        STRDUP(PATHC,PATH);

        int n = getPATHNumDirs();

        char **tokens = NULL;
        MALLOC(tokens,sizeof(char*) * (n+1));

        char *token = strtok(PATHC,":");
        int i = 0;

        while(token != NULL)
        {
                tokens[i] = strdup(token);
                token = strtok(NULL,":");
                i++;
        }
        tokens[n] = NULL;
        free(PATHC);
        return tokens;
}

/*
 * appendToAllPaths
 *
 * @brief: 	This function appends /s to all the paths
 *
 * @param line:		char** paths to append to.
 *
 * @param s:		char* s, the string to append with.
 *
 * @return:		On failure NULL, o.w. the updated paths arr.
 *
 */
char** appendToAllPaths(char **paths, char* s)
{
        if (paths == NULL || s == NULL)
                return NULL;

        char **p = paths;
        int s_len = strlen(s), p_len = 0,i = 0;

        while (*p)
        {
                p_len = strlen(*p);
                char *new_p = malloc(sizeof(char) * (p_len + s_len + 2));
                if (new_p == NULL)
                        goto ERROR_FREE_AND_EXIT;
                strcpy(new_p,*p);
                new_p[p_len] = '/';
                strcpy(new_p + p_len + 1, s);
                new_p[p_len + s_len + 1] = '\0';
                free(paths[i]);
                paths[i] = new_p;
                i++;
                p++;
        }
        return paths;

        ERROR_FREE_AND_EXIT:
                freeArgs(paths);
                return NULL;
}

/*
 * getNumPieces
 *
 * @brief: 	This function returns the number of pieces
 * 		that exists in a given cmd line string
 *
 * @param line:		char*, the line given to the cmd
 *
 * @return:		On failure 0, o.w. the number of pieces.
 *
 */
int getNumPieces(char *str)
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
 * getPiece
 *
 * @brief: 	This function returns a the NEXT piece in a given
 * 		string
 *
 * @param line:		char*, the line given to the cmd
 *
 * @return:		On failure NULL, o.w. the next piece.
 *
 */
char* getPiece(char *str)
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
char** getAllPieces(char *line)
{
	if (line == NULL)
		return NULL;
	int n = getNumPieces(line), j = 0;
	size_t i = 0;
	if (n==0)
		return NULL;
	char **pieces;
	MALLOC(pieces,sizeof(char*) * (n + 1));
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
	pieces[n] = NULL;
	return pieces;
}

/*
 * FindInPath
 *
 * @brief: 	This function searchs a file appears in any
 * 		of the PATH dirs as a regular file + executable
 * 		with permissions.
 *
 * @param args:		A char* file, the file.
 *
 * @return:		On failure a NULL
 * 			o.w. the wanted path/file
 *
 */
void* FindInPath(char *file)
{
        if (file == NULL)
                return NULL;

        struct stat sb;
        char ** path_dirs = getPATHDirs();
        char **p = NULL, *p_i_copy = NULL;

        if (path_dirs == NULL)
                goto ERROR_FREE_AND_EXIT;

        path_dirs = appendToAllPaths(path_dirs,file);
        p = path_dirs;

        while (*p)
        {
                if (stat(*p,&sb) == 0 && S_ISREG(sb.st_mode) && (sb.st_mode & S_IXUSR))
                {
                        STRDUP(p_i_copy,*p);
                        goto FREE_AND_EXIT;
                }
                p++;
        }

        ERROR_FREE_AND_EXIT:
                freeArgs(path_dirs);
                return NULL;

        FREE_AND_EXIT:
                freeArgs(path_dirs);
                return p_i_copy;

}
