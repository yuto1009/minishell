/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 14:21:01 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/18 15:31:59 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *new_token(char *word, t_token_kind kind)
{
    t_token *token;

    token = calloc(1, sizeof(*token));
    if (token == NULL)
        fatal_error("calloc");
    token->word = word;
    token->kind = kind;
    return (token);
}

bool is_blank(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

bool consume_blank(char **rest, char *line)
{
    if (is_blank(*line))
    {
        while (*line && is_blank(*line))
            line++;
        *rest = line;
        return (true);
    }
    *rest = line;
    return (false);
}

bool startswith(const char *s, const char *keyword)
{
    return (ft_memcmp(s, keyword, ft_strlen(keyword)) == 0);
}

bool is_operator(const char *s)
{
    static const char *operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t i = 0;

    while (i < sizeof(operators) / sizeof(*operators)) 
    {
        if (startswith(s, operators[i]))
            return (true);
        i++;
    }
    return (false);
}

bool is_metacharacter(char c)
{
    return (c && ft_strchr("|&;()<> \t\n",c));
}

bool is_word(const char *s)
{
    return (*s && !is_metacharacter(*s));
}

t_token *operator(char **rest, char *line)
{
    static const char *operators[] =  {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t i = 0;
    char *op;
    
    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (startswith(line, operators[i]))
        {
            op = ft_strdup(operators[i]);
            if (op == NULL)
                fatal_error("strdup");
            *rest = line + ft_strlen(op);
            return (new_token(op, TK_OP));
        }
        i++;
    }
    assert_error("Unexpected operator");
}

t_token *word(char **rest, char *line)
{
    const char *start = line;
    char *word;

    while (*line && !is_metacharacter(*line))
        line++;
    word = strndup(start, line - start);    // 作る?
    if (word == NULL)
        fatal_error("strndup");
    *rest = line;
    return (new_token(word, TK_WORD));
}

t_token *tokenize(char *line)
{
    t_token head;
    t_token *token;

    head.next = NULL;
    token = &head;
    while (*line)
    {
        if (consume_blank(&line, line))
            continue;
        else if (is_operator(line))
            token = token->next = operator(&line, line);
        else if (is_word(line))
            token = token->next = word(&line, line);
        else   
            assert_error("Unexpected Token");
    }
    token->next = new_token(NULL, TK_EOF);
    return (head.next);
}

char **tail_recursive(t_token *token, int nargs, char **argv)
{
    if (token == NULL || token->kind == TK_EOF)
        return (argv);
    argv = reallocf(argv, (nargs+2) * sizeof(char *));
    argv[nargs] = ft_strdup(token->word);
    if (argv[nargs] == NULL)
        fatal_error("strdup");
    argv[nargs + 1] = NULL;
    return (tail_recursive(token->next, nargs+1, argv));
}

char **token_list_to_argv(t_token *token)
{
    char **argv;
    
    argv = calloc(1, sizeof(char *));
    if (argv == NULL)
        fatal_error("calloc");
    return (tail_recursive(token, 0, argv));
}
