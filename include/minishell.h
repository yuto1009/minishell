/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:09:07 by yutoendo          #+#    #+#             */
/*   Updated: 2023/11/06 17:33:13 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include <readline/readline.h> // readline, add_history
#include <stdlib.h> // free exit getenv
#include <unistd.h> // fork execve access 
#include <stdbool.h> // boolean
#include <sys/wait.h> // wait
// #include <limits.h>

# define MINISHELL_ERROR 255

typedef int token_kind;
typedef struct s_token t_token;
struct s_token {
    char *str;
    token_kind kind;
    t_token *next;
};

// error.c
void set_output_destination(FILE *dst);
void fatal_error(char *message);
void minishell_error(char *message);
void error_exit(char *location, char *message, int exit_status);

// tokenize.c
bool is_operator(char *line);
int is_blank(char c) ;
bool is_metacharacter(char c);
t_token *new_token(char *str, token_kind kind);
t_token *tokenize_operator(char **line);
t_token *tokenize_word(char **line);
t_token *tokenize(char *line);
char **token_to_argv(t_token *token);

# define TK_WORD 0
# define TK_OPERATOR 1
# define TK_EOF 2




#endif