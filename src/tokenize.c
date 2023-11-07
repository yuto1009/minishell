/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 18:57:26 by yutoendo          #+#    #+#             */
/*   Updated: 2023/11/07 21:17:02 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool is_operator(char *line)
{
    const char *operators[] = {"||", "&&", "&", ";", ";;", "(", ")", "|", "\n"};

    size_t i = 0;
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
    return (c == ' ' || c == '\t' || c == '\n');
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
    size_t i;

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

char *remove_single_quote(char *word)
{
    char *trimmed_word;
    char *open_quote = ft_strchr(word, SINGLE_QUOTE);
    char *close_quote = ft_strrchr(word, SINGLE_QUOTE);

    if (ft_strchr(word, SINGLE_QUOTE) == NULL)
        return (word);
    if (ft_strchr(word, SINGLE_QUOTE) == ft_strrchr(word, SINGLE_QUOTE))
        TODO("Unclosed single quote");
    
}

char *remove_double_quote(char *word)
{
    
}

char *remove_quote(char *word)
{
    size_t i;

    if (word == NULL)
        return (NULL);
    i = 0;
    while (word[i])
    {
        if (word[i] == SINGLE_QUOTE)
        {
            word = remove_single_quote(word);
            break;
        }
        if (word[i] == DOUBLE_QUOTE)
        {
            word = remove_double_quote(word);
            break;
        }
        i++;
    }   
    return (word);
}

t_token *tokenize_word(char **line)
{
    char *word;
    int i;

    word = NULL;
    i = 0;
    while ((*line)[i] != '\0' && is_metacharacter((*line)[i]) == false)
    {
        i++;
    }
    word = ft_substr(*line, 0, i);
    if (word == NULL)
        fatal_error("Malloc Error");
    word = remove_quote(word);
    if (ft_strchr(word, SINGLE_QUOTE))
        remove_single_qupte();
    if (ft_strchr(word, DOUBLE_QUOTE))
        remove_double_quote();
    *line += ft_strlen(word);   // 入力から得た文字列をインクリメント
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