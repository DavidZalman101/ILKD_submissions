//*********************************************
//	  	    UTILITIES
//*********************************************

// INCLUDES
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

	// Home Directory Substitution on the pieces
	subHomeDir_args(pieces);

	return pieces;
}

/*
 * subHomeDir_str
 *
 * @brief:		This function performs home directory
 * 			substitution on a string.
 *
 * 			Examples:
 * 				* ~ -> /home/usr
 * 				* ~red5 -> /home/red5
 * 				* ~/red5/cool/secret/path -> /home/red5/cool/secret/path
 * 				* ~/cool/secret/path -> /home/usr/cool/secret/path
 *
 * @param args:		A char* str.
 *
 * @return:		On failure a NULL
 * 			o.w. a new home_dir_sub str.
 *
 * Note: s is not freed nor changed, it is up to you to manage it!
 *
 */
char* subHomeDir_str(char *s)
{
        if (s == NULL || s[0] != '~')
                return strdup(s);

        char* new_s = NULL, *HOME = getenv("HOME"), *usr_name = NULL;
        int new_s_len = 0, HOME_len = strlen(HOME), s_len = strlen(s), usr_name_len = 0;

        if (s_len == 1)
		STRDUP(new_s,HOME);

        else if (s_len > 1 && s[1] == '/')
        {
                /* Empty username string ~/more/tokens/here */
                new_s_len = HOME_len + s_len - 1;
		MALLOC(new_s,sizeof(char) * (new_s_len+1));
                strcpy(new_s,HOME);
                strcpy(new_s + HOME_len, s+1);
                new_s[new_s_len] = '\0';
        }
        else
        {
                /* Not an Empty username string*/
                char *end_name = strchrnul(s, '/');
                usr_name_len = (end_name - s - 1);
                usr_name = strndup(s+1,usr_name_len);
		if (usr_name == NULL)
			return NULL;
                usr_name[usr_name_len]='\0';

                struct passwd *data = getpwnam(usr_name);
                if (data == NULL)
                {
                        free(usr_name);
                        return strdup(s);
                }
                new_s_len = s_len - usr_name_len - 1 + strlen(data->pw_dir);
                new_s = malloc(sizeof(char) * new_s_len + 1);
		if (new_s == NULL)
		{
			free(usr_name);
			return NULL;
		}
                strcpy(new_s,data->pw_dir);
                strcpy(new_s + strlen(data->pw_dir),s+usr_name_len+1);
                new_s[new_s_len]='\0';
                free(usr_name);
        }
        return new_s;
}

/*
 * subHomeDir_args
 *
 * @brief:		This function performs subHomeDir_str on
 * 			each str in args
 *
 * @param args:		A char** args
 *
 * @return:		nothing, changes args elements if needed.
 *
 */
void subHomeDir_args(char **args)
{
	if (args == NULL)
		return;

	char **args_ptr = args;
	char *s = NULL;

	while (*args_ptr)
	{
		if ( (s = subHomeDir_str(*args_ptr)) != NULL)
		{
			free(*args_ptr);
			*args_ptr=s;
		}
		args_ptr++;
	}
	return;
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
/*
 * CleanCmdFromDirectionFiles
 *
 * @brief: 	This function will create a copy of a given
 * 		file, but without any of the in\out redirection
 * 		including thier files.
 *
 * @param line:		char* line, the line to process.
 *
 * @return: 	On sucsses, a pointer to the clean line
 * 		o.w. NULL.
 */
char* CleanCmdFromDirectionFiles(char* line)
{
	if (line == NULL)
		return NULL;

	int line_len = 0;
	char *ptr = line;
	while (*ptr)
	{
		line_len++;
		ptr++;
	}

	char *leftmost_in = NULL, *rightmost_in = NULL;
	char *leftmost_out = NULL, *rightmost_out = NULL;

	leftmost_in = strchr(line,'<');
	leftmost_out = strchr(line,'>');

	char *left_most=NULL;
	if (leftmost_out == NULL && leftmost_in == NULL)
		return strdup(line);
	else if (leftmost_out == NULL && leftmost_in != NULL)
		left_most = leftmost_in;
	else if (leftmost_out != NULL && leftmost_in == NULL)
		left_most = leftmost_out;
	else
		left_most = leftmost_in < leftmost_out ? leftmost_in : leftmost_out;

	int r_i = 0;
	rightmost_in = GetDirectionFile(line, &r_i, '<');
	int r_o = 0;
	rightmost_out = GetDirectionFile(line, &r_o, '>');

	char *right_most = rightmost_in+r_i > rightmost_out+r_o ? rightmost_in+r_i : rightmost_out+r_o;

	int clean_line_len = line_len - (right_most - left_most);

	char *clean_line = malloc(sizeof(char) * clean_line_len + 1);

	strncpy(clean_line,line,left_most-line);
	strncpy(clean_line+(left_most-line),right_most,line+line_len-right_most);
	clean_line[clean_line_len]='\0';
	return clean_line;
}

/*
 * GetDirectionFile
 *
 * @brief: 	This function will check if there is a
 * 		redirection in a given file.
 * 		If there is, it will return the file pointer
 * 		in the line given, Also, will assign the length
 * 		of the files name in the len var given.
 *
 * @param line:		char* line, the line to check.
 *
 * @param len:		int* len, the len of the file name
 * 			if found, will be assigned.
 *
 * @param direction:	The direction which is wanted to be checked
 * 			Can be either > or <.
 *
 * @return:		If no redirection was found, NULL will be returned
 * 			o.w. will retrun the pointer to the file to redirect to.
 *
 * @Note:		If there was a redirection sign, but no file found
 * 			the pointer will not neccesarly be NULL, but the len will be 0!!!
 *
 * 			It is up to you to make sure and check!
 *
 */
char* GetDirectionFile(char* line, int* len, char direction)
{
	if (direction != '<' && direction != '>')
		return NULL;

	if (line == NULL)
		return NULL;

	/* Input file must be from the right most */
	char *last_occur = strrchr(line,direction);
	char *l = last_occur;

	/* No redirection found */
	if (last_occur == NULL)
		return NULL;

	l++;
	/* Find the begining of the input file */
	while (*l == ' ')
		l++;

	if (*l == '\0')
		return NULL;

	/* Find the end of the input file */
	char *r = l;
	while ( *r && *r != ' ' && *r != '>' && *r != '<' && *r != '\n')
		r++;

	/* Extract the file */

	*len = r-l;
	return l;
}

char **getPIPEDlines(char *line)
{
        if (line == NULL)
                return NULL;
        char *thresh_hold = strchr(line,'|');
        if (thresh_hold == NULL)
                return NULL;
        char **tokens = malloc(sizeof(char*) * 3);
        tokens[0] = strndup(line,thresh_hold-line);
        tokens[1] = strndup(thresh_hold+1,line+strlen(line)-thresh_hold);
	tokens[2] = NULL;
        return tokens;
}

char *getCleanExecline(char *line)
{
	if (line == 0)
		return NULL;

	char *exec_ptr = strstr(line,"exec");
	if (exec_ptr == NULL)
		return NULL;
	char *clean_line = strdup(exec_ptr+4);
	return clean_line;
}
