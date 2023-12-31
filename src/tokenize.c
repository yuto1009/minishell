/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 18:57:26 by yutoendo          #+#    #+#             */
/*   Updated: 2023/12/31 19:56:15 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool is_operator(char *line)
{
    const char *operators[] = {"||", "&&", "&", ";", ";;", "(", ")", "|", "\n"};

    size_t i = 0;
    while (i < sizeof(operators) / sizeof(*operators))//comment by kyoshida iを配列の要素数より小さいだけ回している？
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
        fatal_error("malloc error");
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
        fatal_error("malloc error");
    *line += ft_strlen(operator);
    return (new_token(operator, TK_OPERATOR));
} 

t_token *tokenize_word(char **line)
{
    const size_t word_size = ft_strlen(*line) + 1;
    char *word;
    size_t i;
    size_t j;

    word = (char *)ft_calloc(word_size, sizeof(char));
    if (word == NULL)
        fatal_error("malloc error");
    i = 0;
    j = 0;
    while ((*line)[i] != '\0' && is_metacharacter((*line)[i]) == false)
    {
        if ((*line)[i] == SINGLE_QUOTE || (*line)[i] == DOUBLE_QUOTE)
        {
            const char current_quote = (*line)[i];
            i++;    // クオートをスキップ
            while ((*line)[i] != '\0' && (*line)[i] != current_quote)
            {
                word[j] = (*line)[i];
                i++;
                j++;
            }
            if ((*line)[i] == '\0')
            {
                free(word);
                minishell_error("unclosed quote");
            }
            i++;    // 閉じクオートスキップ
        }
        else
        {
            word[j] = (*line)[i];
            i++;
            j++;
        }
    }
    word[j] = '\0';
    *line += i;   // 入力から得た文字列をインクリメント
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
        fatal_error("malloc error");
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