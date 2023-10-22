/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:55:50 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/22 18:36:01 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

# define ERROR_PREFIX "minishell: "

bool syntax_error = false;

static void perror_prefix(void)
{
    dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
}

void fatal_error(const char *msg)
{
    perror_prefix();
    dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void assert_error(const char *msg)
{
    perror_prefix();
    dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
    exit(255);   
}

void err_exit(const char *location, const char *msg, int status)
{
    perror_prefix();
    dprintf(STDERR_FILENO, "%s: %s\n", location, msg);
    exit(status);   
}

void todo(const char *msg)
{
    perror_prefix();
    dprintf(STDERR_FILENO, "TODO: %s\n", msg);
    exit(255);
}

void    tokenize_error(const char *location, char **rest, char *line)
{
    syntax_error = true;
    dprintf(STDERR_FILENO, "syntax error near unexpected character '%c' in %s\n", *line, location);
    while (*line)
        line++;
    *rest = line;
}

void parse_error(const char *location, t_token **rest, t_token *token)
{
    syntax_error = true;
    perror_prefix();
    dprintf(STDERR_FILENO, "syntax error near unexpected token '%s' in %s\n", token->word, location);
    while (token != NULL && at_eof(token) != true)
        token = token->next;
    *rest = token;
}

void xperror(const char *location)
{
    perror_prefix();
    perror(location);
}