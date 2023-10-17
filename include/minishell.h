/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:58 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/17 22:59:36 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
    #define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../libft/libft.h"

// error.c
void todo(const char *msg) __attribute__((nonreturn));
void fatal_error(const char *msg) __attribute__((nonreturn));
void assert_error(const char *msg) __attribute__((nonreturn));
void err_exit(const char *location, const char *msg, int status) __attribute__((nonreturn));

// tokenize.c
typedef enum e_token_kind {
    TK_WORD,
    TK_RESERVED,
    TK_OP,
    TK_EOF,
} t_token_kind;

typedef struct s_token {
    char *word;
    t_token_kind kind;
    t_token *next;
} t_token;

t_token *tokenize(char *line);
char **token_list_to_argv(t_token *token);

// expand.c
void expand(t_token *token);

// destructor.c
void free_token(t_token *token);
void free_argv(char **argv);

#endif