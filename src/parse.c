/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 16:48:25 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/24 17:00:36 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool equal_op(t_token *token, char *op);
void append_node(t_node **node, t_node *element);

// <simple_command> = <command_element>+
// <command_element> = <word> | <redirection>
// <redirection> = '>' <word>
//               | '<' <word>
//               | '>>' <word>
//               | '<<' <word>

t_node *parse(t_token *token)
{
    t_node *node;
    
    node = new_node(ND_SIMPLE_CMD);
    append_command_element(node, &token, token);
    while (token != NULL && at_eof(token) != true)
    {
        append_command_element(node, &token, token);
    }
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
    if (node == NULL)
    {
        *node = element;
        return ;
    }
    append_node(&(*node)->next, element);
}