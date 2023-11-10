#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "libft/libft.h"
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '\"'

#include <stdio.h>

char *	*word(char **rest, char *line)
{
	const char	*start = line;
	char		*word;
	while (*line && !is_metacharacter(*line))
	{
		if (*line == SINGLE_QUOTE_CHAR)
		{
			// skip quote
			line++;
			while (*line != SINGLE_QUOTE_CHAR)
			{
				if (*line == '\0')
					todo("Unclosed single quote");
				line++;
			}
			// skip quote
			line++;
		}
		else if (*line == DOUBLE_QUOTE_CHAR)
		{
			// skip quote
			line++;
			while (*line != DOUBLE_QUOTE_CHAR)
			{
				if (*line == '\0')
					todo("Unclosed double quote");
				line++;
			}
			// skip quote
			line++;
		}
		else
			line++;
	}
	word = strndup(start, line - start);
	return (word);
}

int main(void)
{
    
    return (0);
}
