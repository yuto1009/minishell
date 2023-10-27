/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 23:34:54 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/27 11:54:34 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int last_status = 127;

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
    new_word = ft_calloc(1, sizeof(char));
    if (new_word == NULL)
        fatal_error("calloc");
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

/*
PARAMETERS
	   A parameter is an entity that stores values.  It can be a name, a
	   number, or one of the special characters listed below under Special
	   Parameters.  A variable is a parameter denoted by a name.  A variable
	   has a value and zero or more attributes.  Attributes are assigned using
	   the declare builtin command (see declare below in SHELL BUILTIN
	   COMMANDS).
	   A parameter is set if it has been assigned a value.  The null string is
	   a valid value.  Once a variable is set, it may be unset only by using
	   the unset builtin command (see SHELL BUILTIN COMMANDS below).
	   A variable may be assigned to by a statement of the form
              name=[value]
	   If value is not given, the variable is assigned the null string.  All
	   values undergo tilde expansion, parameter and variable expansion,
	   command substitution, arithmetic expansion, and quote removal (see
	   EXPANSION below).  If the variable has its integer attribute set, then
	   value is evaluated as an arithmetic expression even if the $((...))
	   expansion is not used (see Arithmetic Expansion below).  Word splitting
	   is not performed, with the exception of "$@" as explained below under
	   Special Parameters.  Pathname expansion is not performed.  Assignment
	   statements may also appear as arguments to the alias, declare, typeset,
	   export, readonly, and local builtin commands.
	   In the context where an assignment statement is assigning a value to a
	   shell variable or array index, the += operator can be used to append to
	   or add to the variable's previous value.  When += is applied to a
	   variable for which the integer attribute has been set, value is
	   evaluated as an arithmetic expression and added to the variable's
	   current value, which is also evaluated.  When += is applied to an array
	   variable using compound assignment (see Arrays below), the variable's
	   value is not unset (as it is when using =), and new values are appended
	   to the array beginning at one greater than the array's maximum index.
	   When applied to a string-valued variable, value is expanded and appended
	   to the variable's value.
*/

bool is_alpha_under(char c)
{
    return (ft_isalpha(c) || c == '_');
}

bool is_alpha_num_under(char c)
{
    return (is_alpha_under(c) || ft_isdigit(c));
}

bool is_variable(char *s)
{
    return (s[0] == '$' && is_alpha_under(s[1]));
}

bool is_special_parameter(char *s)
{
    return (s[0] == '$' && s[1] == '?');
}

void append_num(char **dst, unsigned int num)
{
    if (num == 0)
    {
        append_char(dst, '0');
        return ;
    }
    if (num / 10 != 0)
    {
        append_num(dst, num / 10);
    }
    append_char(dst, '0' + (num % 10));
}

void expand_special_parameter_str(char **dst, char **rest, char *p)
{
    if (is_special_parameter(p) != true)
        assert_error("Expected special parameter");
    p += 2;
    append_num(dst, last_status);
    *rest = p;
}

void expand_variable_str(char **dst, char **rest, char *p)
{
    char *name;
    char *value;
    
    name = ft_calloc(1, sizeof(char));
    if (name == NULL)
        fatal_error("calloc");
    if (*p != '$')
        assert_error("Expected dollar sign");
    p++;
    if (is_alpha_under(*p) != true)
        assert_error("Variable must starts with alphabetical character or underscore");
    append_char(&name, *p++);
    while (is_alpha_num_under(*p))
    {
        append_char(&name, *p++);
    }
    value = getenv(name);
    free(name);
    if (value != NULL)
    {
        while (*value != '\0')
        {
            append_char(dst, *value++);
        }
        *rest = p;
    }
}

void append_single_quote(char **dst, char **rest, char *p)
{
    if (*p == SINGLE_QUOTE_CHAR)
    {
        //　クオートをスキップ
        append_char(dst, *p++);
        while (*p != SINGLE_QUOTE_CHAR)
        {
            if (*p == '\0')
                assert_error("Unclosed single quote");
            append_char(dst, *p++);
        }
        // クオートをスキップ
        append_char(dst, *p++);
        *rest = p;
    }
    else
    {
        assert_error("Expexted single quote");
    }
}

void append_double_quote(char **dst, char **rest, char *p)
{
    if (*p == DOUBLE_QUOTE_CHAR)
    {
        //　クオートをスキップ
        append_char(dst, *p++);
        while (*p != DOUBLE_QUOTE_CHAR)
        {
            if (*p == '\0')
                assert_error("Unclosed double quote");
            else if (is_variable(p) == true)
            {
                expand_variable_str(dst, &p, p);
            }
            else if (is_special_parameter(p))
            {
                expand_special_parameter_str(dst, &p ,p);
            }
            else
            {
                append_char(dst, *p++);
            }
        }
        // クオートをスキップ
        append_char(dst, *p++);
        *rest = p;
    }
    else
    {
        assert_error("Expexted double quote");
    }
}

void expand_variable_token(t_token *token)
{
    char *new_word;
    char *p;
    
    if (token == NULL || token->kind != TK_WORD || token->word == NULL)
        return ;
    p = token->word;
    new_word = ft_calloc(1, sizeof(char));
    if (new_word == NULL)
        fatal_error("calloc");
    while (*p != '\0' && is_metacharacter(*p) != true)
    {
        if (*p == SINGLE_QUOTE_CHAR)
        {
            append_single_quote(&new_word, &p, p);
        }
        else if (*p == DOUBLE_QUOTE_CHAR)
        {
            append_double_quote(&new_word, &p, p);
        }
        else if (is_variable(p) == true)
        {
            expand_variable_str(&new_word, &p, p);
        }
        else if (is_special_parameter(p) == true)
        {
            expand_special_parameter_str(&new_word, &p, p);
        }
        else
        {
            append_char(&new_word, *p++);
        }
    }
    free(token->word);
    token->word = new_word;
    expand_variable_token(token->next);
}

void expand_variable(t_node *node)
{
    if (node == NULL)
        return ;
    expand_variable_token(node->args);
    expand_variable_token(node->filename);
    expand_variable(node->redirects);
    expand_variable(node->command);
    expand_variable(node->next);
}

void expand(t_node *node)
{
    expand_variable(node);
    expand_quote_removal(node);
}

char *expand_heredoc_line(char *line)
{
    char *new_word;
    char *p;
    
    p = line;
    new_word = ft_calloc(1, sizeof(char));
    if (new_word == NULL)
        fatal_error("calloc");
    while (*p != '\0')
    {
        if (is_variable(p) == true)
        {
            expand_variable_str(&new_word, &p, p);
        }
        else if (is_special_parameter(p))
        {
            expand_special_parameter_str(&new_word, &p, p);
        }
        else
        {
            append_char(&new_word, *p++);
        }
    }
    free(line);
    return (new_word);
}