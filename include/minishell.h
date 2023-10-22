/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:58 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/22 18:16:31 by yutoendo         ###   ########.fr       */
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
#include <stdbool.h>
#include "../libft/libft.h"

# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '"'

typedef struct s_token t_token;
typedef enum e_token_kind t_token_kind;
typedef enum e_node_kind t_node_kind;
typedef struct s_node t_node;

// error.c
extern bool syntax_error;
void todo(const char *msg) __attribute__((noreturn));
void fatal_error(const char *msg) __attribute__((noreturn));
void assert_error(const char *msg) __attribute__((noreturn));
void err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));
void tokenize_error(const char *location, char **rest, char *line);
void parse_error(const char *location, t_token **rest, t_token *token);
void xperror(const char *location);

// tokenize.c

typedef enum e_token_kind {
    TK_WORD,    // 単語
    TK_RESERVED,    // 予約語
    TK_OP,  // 演算子
    TK_EOF, // ファイルの終端
} t_token_kind;

typedef struct s_token t_token;

struct s_token {
    char *word;
    t_token_kind kind;
    t_token *next;
};

typedef enum e_node_kind {
    ND_SIMPLE_CMD,
} t_node_kind;

typedef struct s_node t_node;
struct s_node {
    t_token *args;
    t_node_kind kind;
    t_node *next;
};

t_token *tokenize(char *line);
char **token_list_to_argv(t_token *token);
t_token *new_token(char *word, t_token_kind kind);
bool is_blank(char c);
bool consume_blank(char **rest, char *line);
bool startswith(const char *s, const char *keyword);
bool is_operator(const char *s);
bool is_metacharacter(char c);
bool is_word(const char *s);
t_token *operator(char **rest, char *line);
t_token *word(char **rest, char *line);

// expand.c
void expand(t_node *node);

// destructor.c
void free_token(t_token *token);
void free_argv(char **argv);
void free_node(t_node *node);

// parse.c
t_node *parse(t_token *token);
bool at_eof(t_token *token);
t_node *new_node(t_node_kind kind);
void append_token(t_token **tokens, t_token *token);
t_token *tokendup(t_token *token);

#endif