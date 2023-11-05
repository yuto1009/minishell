/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 18:57:26 by yutoendo          #+#    #+#             */
/*   Updated: 2023/11/05 23:40:38 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
        fatal_error("Malloc Error");
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
        fatal_error("Malloc Error");
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
        fatal_error("Malloc Error");
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
    
}