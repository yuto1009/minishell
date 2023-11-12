#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "libft/libft.h"
# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_quotes(char *word)
{
    const int result_size = ft_strlen(word) + 1;
    char *result;
    char *trimmed;
    size_t i;
    size_t str_size;

    result = (char *)ft_calloc(result_size, sizeof(char));
    if (result == NULL)
        exit(EXIT_FAILURE);
    i = 0;
    while(word[i] != '\0')
    {
        str_size = 0;
        if (word[i] != '\0' && word[i] == SINGLE_QUOTE)
        {
            i++;
            while (word[i] != '\0' && word[i] != SINGLE_QUOTE)
            {
                str_size++;
                i++;
            }
            if (word[i] == '\0' || word[i] != SINGLE_QUOTE)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                trimmed = ft_substr(word, i - str_size, str_size);
                if (trimmed == NULL)
                    exit(EXIT_FAILURE);
                ft_strlcat(result, trimmed, result_size);
                free(trimmed);
            }
        }
        str_size = 0;
        if (word[i] != '\0' && word[i] == DOUBLE_QUOTE)
        {
            i++;
            while (word[i] != '\0' && word[i] != DOUBLE_QUOTE)
            {
                str_size++;
                i++;
            }
            if (word[i] == '\0' || word[i] != DOUBLE_QUOTE)
            {
                exit(EXIT_FAILURE);
            }
            else
            {
                trimmed = ft_substr(word, i - str_size, str_size);
                if (trimmed == NULL)
                    exit(EXIT_FAILURE);
                ft_strlcat(result, trimmed, result_size);
                free(trimmed);
            }
        }
        printf("%s\n", result);
    }
    // minishellの場合　free(word); した方がよくね？
    return (result);
}

// int main() {
//     // char input[] = "\'\'\"\"\"\'Hello\'\"\"\"\'\'";
//     char input[] = "\"\'H\'\"e\"l\"o\"\'o\'\"";
//     // Original: "'H'"e"l"o"'o'"
//     // Modified: 'H'elo'o'
//     char *output = remove_quotes(input);

//     if (output != NULL) {
//         printf("Original: %s\n", input);
//         printf("Modified: %s\n", output);
//         free(output);
//     } else {
//         printf("Memory allocation failed.\n");
//     }

//     return 0;
// }
