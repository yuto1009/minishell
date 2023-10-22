/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 23:34:54 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/22 12:47:58 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void append_char(char **s, char c)
{
    size_t size;
    char *new;

    size = 2;
    if (*s)
        size += ft_strlen(*s);
    new = malloc(size);
    if (new == NULL)
        fatal_error("malloc");
    if (*s)
        ft_strlcpy(new, *s, size);
    new[size - 2] = c;
    new[size - 1] = '\0';
    if (*s)
        free(*s);
    *s = new;
}

void quote_removal(t_token *token)
{
    char *new_word;
    char *p;
    
    if (token == NULL || token->kind != TK_WORD || token->word == NULL)
        return;
    p = token->word;
    new_word = NULL;
    while(*p && !is_metacharacter(*p))
    {
        if (*p == SINGLE_QUOTE_CHAR)
        {
            p++;    // クオートをスキップ
            while (*p != SINGLE_QUOTE_CHAR)
            {
                if (*p == '\0')
                    assert_error("Unclosed single quote");
                append_char(&new_word, *p++);
            }
            p++;    // クオートをスキップ
        }
        else if (*p == DOUBLE_QUOTE_CHAR)
        {
            p++;    // クオートをスキップ
            while (*p != DOUBLE_QUOTE_CHAR)
            {
                if (*p == '\0')
                    assert_error("Unclosed double quote");
                append_char(&new_word, *p++);
            }
            p++;    // クオートをスキップ
        }
        else
            append_char(&new_word, *p++);
    }
    free(token->word);
    token->word = new_word;
    quote_removal(token->next);
}

void expand(t_token *token)
{
    quote_removal(token);
}
