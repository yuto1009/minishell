/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:09:07 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/23 13:17:28 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include "../built_in/include/built_in.h"
// #include "parser.h"
#include <stdio.h> // DEBUG
# include <signal.h>
#include <readline/readline.h> // readline, add_history
#include <readline/history.h>
#include <stdlib.h> // free exit getenv
#include <unistd.h> // fork execve access 
#include <stdbool.h> // boolean
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <string.h> // strerror
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include <limits.h>

# define MINISHELL_ERROR 255
# define SYNTAX_ERROR 258
# define CMD_NOT_FOUND_ERROR 127
# define GENERAL_ERRORS 1

extern int g_status;

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
    t_node *prev;
    t_node *redirection;
    t_node *next;
    int currentout_fd; // 現在のfd初期値はSTDIN
    int currentin_fd; // 現在のfd初期値はSTDOUT
    int redirout_fd;
    int redirin_fd;
    int pipe_in[2];
    int pipe_out[2];
    int index;

};
//main
void roop_readline(void);

// 環境変数マップ
typedef struct s_var t_var;

struct s_var {
    char *name;
    char *value;
    t_var *prev;
    t_var *next;
};

extern bool is_sig_get;

// volatile sig_atomic_t	sig = 0;
// error.c
void set_output_destination(FILE *dst);
void fatal_error(char *message);
void minishell_error(char *message);
void cmd_error_exit(char *location, char *message, int exit_status);
int syntax_error_exit(char *token_str);
int unsupported_token_msg(char *str);
void cd_error(char *cmd);
// tokenize.c
bool is_operator(char *line);
int is_blank(char c) ;
bool is_metacharacter(char c);
t_token *new_token(char *str, token_kind kind);
t_token *tokenize_operator(char **line);
t_token *tokenize_word(char **line);
t_token *tokenize(char *line);
char **token_to_argv(t_token *token);
int tokenize_error(t_token *token);
int count_token_len(t_token *token);
//parser
t_node *parser(t_token *token);
t_node *get_next_node(t_node *node);

// expand.c
int expand(t_token *token, t_var *env_map,int prev_status);

//pipe_utils.c
void set_pipe(t_node *node);
void dup_child_pipe(t_node *node);
void set_parent_pipe(t_node *node);
void	signal_heredoc(void);

//signal
void	signal_parent_init(void);
void	signal_child_init(void);
void setup_signal();

//redirect
int heredoc(char *delimiter);
void open_file(t_node *node);
void dup_fd(t_node *node);
char **serch_redir(t_node *node,int len);
int exec(t_node *node,t_var *env_map,int prev_status);
void reset_fd(t_node *node);
//path
char *search_path(char *filename);


// map.c
t_var * init_env_map(void);
char *trim_env_name(char *env);
char *trim_env_value(char *env);
char *get_env_value(char *env_name, t_var *map);
char *get_env_list(t_var *map);
void unset_env(char *env_name,t_var *map);
t_var *export_env(t_var *map, char *env_name, char *env_value);


void wait_pid(pid_t pid);

int count_token_len(t_token *token);
//buildin
 bool is_buildin(char  *str);
 int exec_buildin(char **argv,t_var *env_map,int prev_status);

# define TK_WORD 0
# define TK_OPERATOR 1
# define TK_REDIRECTION 2
# define TK_EOF 3

# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'
# define DOLLAR_SIGN '$'
# define EQUAL_SIGN '='
# define QUESTION '?'


#endif