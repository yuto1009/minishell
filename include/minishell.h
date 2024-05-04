/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:09:07 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/04 12:32:21 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../built_in/include/built_in.h"
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// Global variable
extern int				g_status;

// Error
# define MINISHELL_ERROR 255
# define SYNTAX_ERROR 258
# define CMD_NOT_FOUND_ERROR 127
# define GENERAL_ERRORS 1

// Token kind
# define TK_WORD 0
# define TK_OPERATOR 1
# define TK_REDIRECTION 2
# define TK_EOF 3

// Char
# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'
# define DOLLAR_SIGN '$'
# define EQUAL_SIGN '='
# define QUESTION '?'

// Token
typedef struct s_token	t_token;
struct					s_token
{
	char				*str;
	int					kind;
	t_token				*prev;
	t_token				*next;
};

// Node
typedef struct s_node	t_node;

struct					s_node
{
	t_token				*token;
	t_node				*prev;
	t_node				*redirection;
	t_node				*next;
	int					currentout_fd;
	int					currentin_fd;
	int					redirout_fd;
	int					redirin_fd;
	int					pipe_in[2];
	int					pipe_out[2];
};

// Enviroment variable
typedef struct s_var	t_var;

struct					s_var
{
	char				*name;
	char				*value;
	t_var				*prev;
	t_var				*next;
};

// main.c
int						count_token_len(t_token *token);

// interpret.c
void					interpret(char *line, t_var *env_map);

// tokenize_line.c
t_token					*tokenize_line(char **line);

// tokenize_word.c
t_token					*tokenize_word(char **line);

// tokenize_new.c
t_token					*new_token(char *str, int kind);

// tokenize_is.c
bool					is_operator(char *line);
bool					is_redirection_operator(char *line);
int						is_blank(char c);
bool					is_metacharacter(char c);

// tokenize_operator.c
t_token					*tokenize_operator(char **line);
t_token					*tokenize_redirection_operator(char **line);
// tokenize.c
t_token					*tokenize(char *line);

// tokenize_error.c
int						tokenize_error(t_token *token);

// parser.c
t_node					*parser(t_token *token);

// error.c
void					set_output_destination(FILE *dst);
void					fatal_error(char *message);
int						minishell_error(char *message);
int						cd_error(char *cmd);
void					cmd_error_exit(char *location, char *message,
							int exit_status);
int						syntax_error_exit(char *token_str);
int						unsupported_token_msg(char *str);

// expand.c
int						expand(t_token *token, t_var *env_map, int prev_status);

// expand_append.c
void					append_variable(char **str, char **new_str,
							t_var *env_map, int prev_status);

// expand_append_utils.c
void					append_question(char **str, char **new_str,
							int prev_status);
void					append_env_variable(char **str, char **new_str,
							t_var *env_map);
void					append_double_quote(char **str, char **new_str,
							t_var *env_map, int prev_status);
void					append_single_quote(char **str, char **new_str);
void					append_char(char **str, char new_char);

// expand_append_env.c
void					append_env_variable(char **str, char **new_str,
							t_var *env_map);

// expand_remove.c
void					remove_void_tokens(t_token *token);
void					remove_single_quote(char **str, char **new_str);
void					remove_double_quote(char **str, char **new_str);
void					remove_quotes(t_token *token);

// expand_is.c
bool					is_dollar_sign(char c);
bool					is_single_dollar_sign(char *str);
bool					is_env_variable(char *str);
bool					is_alpha(char chr);
bool					is_exit_status(char *str);

// pipe_utils.c
void					set_pipe(t_node *node);
void					dup_child_pipe(t_node *node);
void					set_parent_pipe(t_node *node);

// signal.c
void					signal_heredoc(void);
void					setup_signal(void);
void					sigquit_action(int signum);
//signal_init.c
void					signal_parent_init(void);
void					signal_child_init(void);
void					sigint_action(int signum);

// headoc.c
int						heredoc(char *delimiter);

// redirect.c
void					dup_fd(t_node *node);
void					reset_fd(t_node *node);
int						stashfd(int fd);
char					**search_redir(t_node *node, int len);

// exec_cmd.c
bool					is_builtin(char *str);
int						exec_builtin(char **argv, t_var *env_map,
							int prev_status);
int						exec(t_node *node, t_var *env_map, int prev_status);

/// exec_cmd.c
int						get_listsize(t_var *env_map);
char					*list2join(t_var *map);
char					**envlist2char(t_var *env_map);
void					check_access(char *args);

// search_path.c
char					*search_path(char *filename);

// map.c
t_var					*export_env(t_var *map, char *env_name,
							char *env_value);
char					*trim_env_name(char *env);
char					*trim_env_value(char *env);
void					unset_env(char *env_name, t_var *map);

// map_get.c
char					*get_env_value(char *env_name, t_var *map);

// map_init.c
t_var					*init_env_map(void);

// wait_pid.c
void					wait_pid(pid_t pid);

int						my_strcmp(const char *s1, const char *s2);
#endif
