#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "libft/libft.h"

# define TK_WORD 0
# define TK_OPERATOR 1
# define TK_EOF 2

typedef int token_kind;
typedef struct s_token t_token;
struct s_token {
    char *str;
    token_kind kind;
    t_token *next;
};

bool is_operator(char c)
{
    // const char metacharacters[] = {'|', '&', ';', '(', ')', '<', '>', ' ', '\t'};
    const char *metacharacters = "|&;()<> \t";

    if (ft_strchr(metacharacters, c))
    {
        return (true);
    }
    return (false);
}

t_token *new_token(char *str, token_kind kind)
{
    t_token *token;

    token = (t_token *)ft_calloc(1, sizeof(t_token));
    if (token == NULL)
        exit(EXIT_FAILURE);
    token->str = str;
    token->kind = kind;
    token->next = NULL;
    return (token);
}

t_token *tokenize_operator(char **line)
{
    const char *metacharacters = "|&;()<> \t";
    size_t operator_size;

    operator_size = 0;
    while (**line != '\0')
    {
        if (!ft_strchr(metacharacters, **line))
        {
            break;
        }
        operator_size++;
        (*line)++;
    }
    return (new_token(ft_substr(*line, 0, operator_size), TK_OPERATOR));
} 

t_token *tokenize_word(char **line)
{
    size_t word_size;

    word_size = 0;
    while (**line != '\0' && is_operator(**line) != true)
    {
        word_size++;
        (*line)++;
    }
    return (new_token(ft_substr(*line, 0, word_size), TK_WORD));
}

t_token *tokenize(char *line)
{
    t_token *head;
    t_token *current;
    t_token *new;

    head = NULL;
    current = NULL;
    if (line == NULL)
        exit(EXIT_FAILURE);
    while (*line != '\0')
    {
        if (ft_isprint(*line) != true)
        {
            line++;
        }
        else
        {
            if (is_operator(*line))
            {
                new = tokenize_operator(&line);
            }
            else
            {
                new = tokenize_word(&line);
            }
            if (head == NULL)
            {
                head = new;
                current = new;
            }
            else
            {
                 current->next = new;
            }
        }
    }
    if (head == NULL)
    {
        head = new_token(NULL, TK_EOF);
    }
    else
    {
        current->next = new_token(NULL, TK_EOF);
    }
    return (head);
}

int main(void)
{
    t_token *token;
    char *line = "echo \"Hello World\"";

    token = NULL;
    token = tokenize(line);
    // puts("DEBUG");
    printf("%s\n", line);
    while (token)
    {
        printf("token type is %d\n", token->kind);
        token = token->next;
    }
    return (0);
}