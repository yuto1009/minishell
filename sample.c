#include "../include/minishell.h"

char *remove_quotes(const char *word) {

    const size_t result_size = ft_strlen(word) + 1;
    char *result;
    size_t i;
    size_t j;
    
    if (word == NULL) 
        return NULL;
    result = (char *)malloc(result_size * sizeof(char));
    if (result == NULL) 
        puts("malloc error");
    i = 0;
    j = 0;
    while (word[i] != '\0') {
        if (word[i] == SINGLE_QUOTE || word[i] == DOUBLE_QUOTE) 
        {
            const char current_quote = word[i++];
            while (word[i] != '\0' && word[i] != current_quote) 
            {
                result[j++] = word[i++];
            }
            if (word[i] == '\0') 
            {
                free(result);
                puts("unclosed quote");
            }
            i++; // 閉じクオートをスキップ
        } else 
        {
            result[j++] = word[i++];
        }
    }
    // free((void *)word);
    result[j] = '\0';
    return result;
}

int main(void)
{
    char p[] = "echo \"Hello World\"";
    char *res = remove_quotes(p);

    printf("result: %s\n", res);

    return (0);
}
