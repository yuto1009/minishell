/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:09:07 by yutoendo          #+#    #+#             */
/*   Updated: 2024/02/05 13:23:37 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include "../built_in/include/built_in.h"
// #include "parser.h"
#include <readline/readline.h> // readline, add_history
#include <stdlib.h> // free exit getenv
#include <unistd.h> // fork execve access 
#include <stdbool.h> // boolean
#include <sys/wait.h> // wait
#include <stdio.h> // DEBUG
#include <errno.h> // errno
#include <string.h> // strerror
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include <limits.h>

# define MINISHELL_ERROR 255
# define SYNTAX_ERROR 258

typedef int token_kind;
typedef struct s_token t_token;
struct s_token {
    char *str;
    token_kind kind;
    t_token *prev;
    t_token *next;
};

// typedef struct s_token t_token;
typedef struct s_node t_node;

struct s_node {
    t_token *token;
    t_node *left;
    t_node *right;
    t_node *redirection;
    int current_fd; // 現在のfd初期値はSTDIN
    int redir_fd;

};

// error.c
void set_output_destination(FILE *dst);
void fatal_error(char *message);
void minishell_error(char *message);
void cmd_error_exit(char *location, char *message, int exit_status);
void syntax_error_exit(char *token_str);

// tokenize.c
bool is_operator(char *line);
int is_blank(char c) ;
bool is_metacharacter(char c);
t_token *new_token(char *str, token_kind kind);
t_token *tokenize_operator(char **line);
t_token *tokenize_word(char **line);
t_token *tokenize(char *line);
char **token_to_argv(t_token *token);

//parser
// t_node *parser(t_node *node); 
t_node *parser(t_token *token);


# define TK_WORD 0
# define TK_OPERATOR 1
# define TK_REDIRECTION 2
# define TK_EOF 3

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'



#endif