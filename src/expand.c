/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/03/07 22:18:08 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void append_single_quote(const char **str, char **new_str)
{
    
}

static void append_double_quote(const char **str, char **new_str)
{
    
}

static bool is_dollar_sign(char c)
{
    return (c == '$');
}

static bool is_single_dollar_sign(char *str)
{
    // ToDo check if dollar sign and next is null or space or double quote or single quote
        // then true
}

static void append_variable(const char **str, char **new_str)
{
    // ToDo check if single dollar sign

    // ToDo check if upper char, 
        // then append_env_variable
    
    // ToDo check if lower char,
        // then append_shell_variable
            // not branch 76

    // ToDo check if ?,
        // then append_exit_status
            // not branch 76

    // else minishell_error
}

static char *ft_strcpy(char *dest, const char *src) 
{
    char *original_dest = dest;

    while (*src != '\0') 
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}

static void append_char(const char **str, char new_char)
{
    // ToDo re-calloc new_str by 1
        // copy to new new_str
            // renew the pointer
}

void expand(t_token *token)
{
    char *new_str;
    const char *str = token->str;
    
    if (token == NULL)
        return;
    if (token->kind != TK_WORD || token->str == NULL)
        return expand(token->next);
    new_str = (char *)calloc(1, sizeof(char));
    if (new_str == NULL)
        fatal_error("Malloc Error");
    while(*str != '\0')
    {
        if (*str == SINGLE_QUOTE)
            append_single_quote(&str, &new_str);
        else if (*str == DOUBLE_QUOTE)
            append_double_quote(&str, &new_str);
        else if (is_dollar_sign(*str) == true)
            append_variable(&str, &new_str);
        else
        {
            append_char(&str, *new_str);
            new_str++;
        }
    }
    free(token->str);
    token->str = new_str;
    expand(token->next);
}