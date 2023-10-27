/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 16:48:25 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/27 11:57:43 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool equal_op(t_token *token, char *op);
void append_node(t_node **node, t_node *element);
t_node *pipeline(t_token **rest, t_token *token);
t_node *simple_command(t_token ** rest, t_token *token);

// <pipeline> = <simple_command> ('|' <pipeline>)
// <simple_command> = <command_element>+
// <command_element> = <word> | <redirection>
// <redirection> = '>' <word>
//               | '<' <word>
//               | '>>' <word>
//               | '<<' <word>

t_node *parse(t_token *token)
{
    return (pipeline(&token, token));
}

/*
   Pipelines
	   A pipeline is a sequence of one or more commands separated by the
	   character |.  The format for a pipeline is:
	   [time [-p]] [ ! ] command [ | command2 ... ]
	   The standard output of command is connected via a pipe to the standard
	   input of command2.  This connection is performed before any redirections
	   specified by the command (see REDIRECTION below).
	   The return status of a pipeline is the exit status of the last command,
	   unless the pipefail option is enabled.  If pipefail is enabled, the
	   pipeline's return status is the value of the last (rightmost) command to
	   exit with a non-zero status, or zero if all commands exit successfully.
	   If the reserved word !  precedes a pipeline, the exit status of that
	   pipeline is the logical negation of the exit status as described above.
	   The shell waits for all commands in the pipeline to terminate before
	   returning a value.
	   If the time reserved word precedes a pipeline, the elapsed as well as
	   user and system time consumed by its execution are reported when the
	   pipeline terminates.  The -p option changes the output format to that
	   specified by POSIX.  The TIMEFORMAT variable may be set to a format
	   string that specifies how the timing information should be displayed;
	   see the description of TIMEFORMAT under Shell Variables below.
	   Each command in a pipeline is executed as a separate process (i.e., in a
	   subshell).
*/

t_node *pipeline(t_token **rest, t_token *token)
{
    t_node *node;

    node = new_node(ND_PIPELINE);
    node->inpipe[0] = STDIN_FILENO;
    node->inpipe[1] = -1;
    node->outpipe[0] = -1;
    node->outpipe[1] = STDOUT_FILENO;
    node->command = simple_command(&token, token);
    if (equal_op(token, "|"))
    {
        node->next = pipeline(&token, token->next);
    }
    *rest = token;
    return (node);
}

/*
control operator
	  A token that performs a control function.  It is one of the following symbols:
	  || & && ; ;; ( ) | <newline>
*/

bool is_control_operator(t_token *token)
{
    static const char *operators[] = {"||", "&", "&&", ";", ";;", "(", ")", "|", "\n"};
    size_t i;

    i = 0;
    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (startswith(token->word, operators[i]))
        {
            return (true);
        }
        i++;
    }
    return (false);
}

/*
   Simple Commands
	   A simple command is a sequence of optional variable assignments followed
	   by blank-separated words and redirections, and terminated by a control
	   operator.  The first word specifies the command to be executed, and is
	   passed as argument zero.  The remaining words are passed as arguments to
	   the invoked command.
	   The return value of a simple command is its exit status, or 128+n if the
	   command is terminated by signal n.
*/

t_node *simple_command(t_token **rest, t_token *token)
{
    t_node *node;
    
    node = new_node(ND_SIMPLE_CMD);
    append_command_element(node, &token, token);
    while (token && at_eof(token) != true && is_control_operator(token) != true)
    {
        append_command_element(node, &token, token);
    }
    *rest = token;
    return (node);
}

t_node *redirect_out(t_token **rest, t_token *token)
{
    t_node *node;

    node = new_node(ND_REDIR_OUT);
    node->filename = tokendup(token->next);
    node->target_fd = STDOUT_FILENO;
    *rest = token->next->next;
    return (node);
}

t_node *redirect_in(t_token **rest, t_token *token)
{
    t_node *node;
    
    node = new_node(ND_REDIR_IN);
    node->filename = tokendup(token->next);
    node->target_fd = STDIN_FILENO;
    *rest = token->next->next;
    return (node);
}

t_node *redirect_append(t_token **rest, t_token *token)
{
    t_node *node;

    node = new_node(ND_REDIR_APPEND);
    node->filename = tokendup(token->next);
    node->target_fd = STDOUT_FILENO;
    *rest = token->next->next;
    return (node);
}

t_node *redirect_heredoc(t_token **rest, t_token *token)
{
    t_node *node;
    
    node = new_node(ND_REDIR_HEREDOC);
    node->delimiter = tokendup(token->next);
    if (ft_strchr(node->delimiter->word, SINGLE_QUOTE_CHAR) == NULL && ft_strchr(node->delimiter->word, DOUBLE_QUOTE_CHAR) == NULL)
    {
        node->is_delim_unquoted = true;
    }
    node->target_fd = STDIN_FILENO;
    *rest = token->next->next;
    return (node);
}

void append_command_element(t_node *command, t_token **rest, t_token *token)
{
    if (token->kind == TK_WORD)
    {
        append_token(&command->args, tokendup(token));
        token = token->next;
    }
    else if (equal_op(token, ">") && token->next->kind == TK_WORD)
    {
        append_node(&command->redirects, redirect_out(&token, token));
    }
    else if (equal_op(token, "<") && token->next->kind == TK_WORD)
    {
        append_node(&command->redirects, redirect_in(&token, token));
    }
    else if (equal_op(token, ">>") && token->next->kind == TK_WORD)
    {
        append_node(&command->redirects, redirect_append(&token, token));
    }
    else if (equal_op(token, "<<") && token->next->kind == TK_WORD)
    {
        append_node(&command->redirects, redirect_heredoc(&token, token));
    }
    else
    {
        todo("append_command_element");
    }
    *rest = token;
}

bool at_eof(t_token *token)
{
    return (token->kind == TK_EOF);
}

bool equal_op(t_token *token, char *op)
{
    if (token->kind != TK_OP)
    {
        return (false);
    }
    return (strcmp(token->word, op) == 0);
}

t_node *new_node(t_node_kind kind)
{
    t_node *node;
    
    node = ft_calloc(1, sizeof(*node));
    if (node == NULL)
        fatal_error("calloc");
    node->kind = kind;
    return (node);
}

t_token *tokendup(t_token *token)
{
    char *word;
    
    word = ft_strdup(token->word);
    if (word == NULL)
        fatal_error("strdup");
    return (new_token(word, token->kind));
}

void append_token(t_token **token, t_token *element)
{
    if (*token == NULL)
    {
        *token = element;
        return ;
    }
    append_token(&(*token)->next, element);
}

void append_node(t_node **node, t_node *element)
{
    if (*node == NULL)
    {
        *node = element;
        return ;
    }
    append_node(&(*node)->next, element);
}