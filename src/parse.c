/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 16:48:25 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/22 18:35:14 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node *parse(t_token *token)
{
    t_node *node;
    
    node = new_node(ND_SIMPLE_CMD);
    while (token != NULL && at_eof(token) != true)
    {
        if (token->kind == TK_WORD)
        {
            append_token(&node->args, tokendup(token));
            token = token->next;
        }
        else
        {
            parse_error("Unexpected Token", &token, token);
        }
    }
    return (node);
}

bool at_eof(t_token *token)
{
    return (token->kind == TK_EOF);
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

void append_token(t_token **tokens, t_token *token)
{
    if (*tokens == NULL)
    {
        *tokens = token;
        return ;
    }
    append_token(&(*tokens)->next, token);
}