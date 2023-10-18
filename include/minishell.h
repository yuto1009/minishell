/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:58 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/18 23:44:51 by yutoendo         ###   ########.fr       */
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

// error.c
void todo(const char *msg) __attribute__((noreturn));
void fatal_error(const char *msg) __attribute__((noreturn));
void assert_error(const char *msg) __attribute__((noreturn));
void err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));

// tokenize.c
# define SINGLE_QUOTE_CHAR '\''
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
void expand(t_token *token);

// destructor.c
void free_token(t_token *token);
void free_argv(char **argv);

#endif