/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/01/29 23:11:14 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node *new_node(t_token *axis_token, t_node *left_node, t_node *right_node)
{
    t_node *node;

    node = (t_node *)ft_calloc(1, sizeof(t_node));
    if (node == NULL)
        fatal_error("malloc error");
    if (axis_token != NULL)
        node->token = axis_token;
    else
        node->token = new_token(NULL, TK_EOF);
    
    if (left_node != NULL)
        node->left = left_node;
    else
        node->left = NULL;
    if (right_node != NULL)
        node->right = right_node;
    else
        node->right = NULL;
    
    return (node);
}

// 最高層のノードの左側のトークンを切断する

t_token *split_left_tokens(t_token *token)
{
    t_token *new_eof;

    if (token->prev != NULL)
        token = token->prev; // 最高層ノードの左側のトークンに移動する
    new_eof = new_token(NULL, TK_EOF);
    new_eof->prev = token; // 新しいEOFトークンのprevを設定する
    token->next = new_eof;

    while (token->prev->kind != TK_EOF)
    token = token->prev;
    return token;
}

// 最高層のノードの右側のトークンを切断する

t_token *split_right_tokens(t_token *token)
{
    t_token *prev_token;
    if(token->next->kind == TK_EOF)
        return (new_token(NULL, TK_EOF));   // もし最高層ノードの右側にトークンがなかったらEOFトークンだけ入れる
    token = token->next;    // axisの次のトークンを参照する
    // prev_token = (t_token *)ft_calloc(1, sizeof(t_token));
    prev_token = new_token(NULL, TK_EOF);   // 
    prev_token->next = token;   // 
    token->prev = prev_token;
    return (token);
}

t_token *find_axis_token(t_token *token)
{
    t_token *axis;
    // printf("ok\n");
    axis = NULL;
    // while ((token != NULL && token->next != NULL) || token->kind != TK_EOF)
    while (token != NULL && token->kind != TK_EOF)
    {
    // printf("token->str: %s\n", token->str);
        if (ft_strncmp(token->str, ";", 1) == 0)
            axis = token;
        else if (axis != NULL && ft_strncmp(axis->str, ";", 1) != 0 && ft_strncmp(token->str, "|", 1) == 0)
            axis = token;
        else if(axis == NULL && ft_strncmp(token->str, "|", 1) == 0)
            axis = token;
        token = token->next;
    }
    if (axis == NULL)
        axis = new_token(NULL, TK_EOF);
    return axis;
}

t_node *parser(t_token *token)
{
    t_token *axis_token;
    t_token *left_token;
    t_token *right_token;
    t_node *left_subnode;
    t_node *right_subnode;
    t_node *new_node_instance;

    if (token == NULL || token->kind == TK_EOF)
        return NULL;                                                                                                                    
    axis_token = find_axis_token(token);
    if(axis_token == NULL || axis_token->kind == TK_EOF)
    {
        return new_node(token, NULL, NULL);
    }

    right_token = split_right_tokens(axis_token);
    left_token = split_left_tokens(axis_token);
    // printf("axis : %s , right : %s , left : %s\n",axis_token->str,right_token->str , left_token->str);
    
    left_subnode = parser(left_token);
    right_subnode = parser(right_token);
    new_node_instance = new_node(axis_token, left_subnode, right_subnode);

    return (new_node_instance);
}