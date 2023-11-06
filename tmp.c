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

bool is_operator(char *line)
{
    const char *operators[] = {"||", "&&", "&", ";", ";;", "(", ")", "|", "\n"};

    int i = 0;
    while (i < sizeof(operators) / sizeof(*operators))
    {
       if (ft_strncmp(line, operators[i], ft_strlen(operators[i])) == 0) 
       {
            return (true);
       }
       i++;
    }
    return (false);
}

int is_blank(char c) 
{
    return (c == ' ' || c == '\t');
}

bool is_metacharacter(char c)
{
    const char *metacharacters = "|&;()<> \t";

    if (ft_strchr(metacharacters, c) != NULL)
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
    const char *operators[] = {"||", "&&", ";;", "&", ";", "(", ")", "|", "\n"};
    char *operator;
    int i;

    operator = NULL;
    i = 0;
    while (i < sizeof(operators) / sizeof(*operators))
    {
       if (ft_strncmp(*line, operators[i], ft_strlen(operators[i])) == 0) 
       {
            break;
       }
       i++;
    }
    operator = ft_substr(*line, 0, ft_strlen(operators[i]));
    if (operator == NULL)
        exit(EXIT_FAILURE);
    *line += ft_strlen(operator);
    return (new_token(operator, TK_OPERATOR));
} 

t_token *tokenize_word(char **line)
{
    size_t word_size;
    char *word;
    int i;

    word = NULL;
    word_size = 0;
    i = 0;
    while ((*line)[i] != '\0' && is_metacharacter((*line)[i]) == false)
    {
        i++;
    }
    word = ft_substr(*line, 0, i);
    if (word == NULL)
        exit(EXIT_FAILURE);
    *line += ft_strlen(word);
    return (new_token(word, TK_WORD));
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
        if (is_blank(*line) == true)
            line++;
        else
        {
            if (is_operator(line))
            {
                new = tokenize_operator(&line);
            }
            else
            {
                new = tokenize_word(&line);
            }
        }
        if (head == NULL)
        {
            head = new;
            current = new;
        }
        else
        {
            current->next = new;
            current = current->next;
        }
    }
    if (head == NULL)
        head = new_token(NULL, TK_EOF);
    else
        current->next = new_token(NULL, TK_EOF);
    return (head);
}

char **token_to_argv(t_token *token)
{
    char **argv;
    t_token *head = token;
    size_t token_size;
    size_t i;
    
    token_size = 0;
    while (token->kind != TK_EOF)
    {
        token = token->next;
        token_size++;
    }
    token_size++;
    argv = (char **)calloc(token_size+1, sizeof(char *));
    if (argv == NULL)
        exit(EXIT_FAILURE);
    token = head;
    i = 0;
    while (i < token_size)
    {
        argv[i] = token->str;
        token = token->next;
        i++;
    }
    argv[i] = NULL;
    return (argv);
}

int main(void)
{
    t_token *token;
    char *line = "echo \"Hello World\"";

    token = NULL;
    token = tokenize(line);
    char **argv = token_to_argv(token);
    while(*argv)
    {
        printf("argv: %s\n", *argv);
        argv++;
    }
    return (0);
}
