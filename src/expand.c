/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 23:34:54 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/26 14:43:54 by yutoendo         ###   ########.fr       */
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

void remove_single_quote(char **dst, char **rest, char *p)
{
    if (*p == SINGLE_QUOTE_CHAR)
    {
        // クオートをスキップ
        p++;
        while (*p != SINGLE_QUOTE_CHAR)
        {
            if (*p == '\0')
                assert_error("Unclosed single quote");
            append_char(dst, *p++);
        }
        // クオートをスキップ
        p++;
        *rest = p;
    }
    else
    {
        assert_error("Expected single quote");
    }
}

void remove_double_quote(char **dst, char **rest, char *p)
{
    if (*p == DOUBLE_QUOTE_CHAR)
    {
        // クオートをスキップ
        p++;
        while (*p != DOUBLE_QUOTE_CHAR)
        {
            if (*p == '\0')
            {
                assert_error("Unclosed double quote");
            }
                append_char(dst, *p++);
        }
            // クオートをスキップ
            p++;
            *rest = p;
    }
    else
    {
        assert_error("Expected double quote");
    }
}

void remove_quote(t_token *token)
{
    char *new_word;
    char *p;
    
    if (token == NULL || token->kind != TK_WORD || token->word == NULL)
        return ;
    p = token->word;
    new_word = NULL;
    while (*p && !is_metacharacter(*p))
    {
        if (*p == SINGLE_QUOTE_CHAR)
            remove_single_quote(&new_word, &p, p);
        else if (*p == DOUBLE_QUOTE_CHAR)
            remove_double_quote(&new_word, &p, p);
        else
            append_char(&new_word, *p++);
    }
    free(token->word);
    token->word = new_word;
    remove_quote(token->next);
}

void expand_quote_removal(t_node *node)
{
    if (node == NULL)
        return ;
    remove_quote(node->args);
    remove_quote(node->filename);
    remove_quote(node->delimiter);
    expand_quote_removal(node->redirects);
    expand_quote_removal(node->command);
    expand_quote_removal(node->next);
}

void expand(t_node *node)
{
    expand_quote_removal(node);
}