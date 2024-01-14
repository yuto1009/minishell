/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/01/14 19:26:37 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

typedef struct s_token t_token;
typedef struct s_node t_node;

struct s_node {
    t_token *token;
    t_node *right;
    t_node *left;
};

// 見つけた最高層のノードを軸に子ノードを格納する
t_node *new_node(t_token *axis)
{
    t_node *node;

    node = (t_node *)ft_calloc(1, sizeof(t_node));
    if (node == NULL)
        fatal_error("malloc error");
    node->token = axis;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

// 最高層のノードの左側のトークンを切断する
t_token *split_left_tokens(t_token *token)
{
    if (token->prev != NULL)
    {
        token = token->prev;    // 最高層ノードの左側のトークンに移動する
    }
    token->next = new_token(NULL, TK_EOF);
    while(token->prev != NULL)
    {
        token = token->prev;
    }
    return (token);
}

// 最高層のノードの右側のトークンを切断する
t_token *split_right_tokens(t_token *token)
{
    if(token->next==NULL)
        return (new_token(NULL, TK_EOF));   // もし最高層ノードの右側にトークンがなかったらEOFトークンだけ入れる
    token = token->next;
    return (token);
}

t_token *find_axis_token(t_token *token)
{
    t_token *token;
    t_token *axis;

    axis = NULL;
    while(token->next != NULL)
    {
        if (ft_strncmp(token->str, ';', 1) == 0)
            axis = token;
        else if (ft_strncmp(axis,';',1) != 0 && ft_strncmp(token->str , '|',1) == 0)
            axis = token;
            
        token = token->next;
    }
    return axis;
}
// 色んな関数のまとめ？
t_node *parser(t_token *token, t_node *node)
{
    t_node *node;
    t_token *axis;
    t_token *left;
    t_token *right;
    
    axis = find_highest_node(token);
    if(axis == NULL)
        return;
    left = split_left_tokens(axis);
    right = split_right_tokens(axis);
    node = new_node(axis);
    
}