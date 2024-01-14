/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/01/14 18:49:20 by yuendo           ###   ########.fr       */
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

// 最高層のノードを見つける
t_token *find_node_token(t_token *token)
{
    
}

// 見つけた最高層のノードを軸に子ノードを格納する
t_node *new_node(t_token *token)
{
    t_node *node;

    node = (t_node *)ft_calloc(1, sizeof(t_node));
    if (node == NULL)
        fatal_error("malloc error");
    node->token = token;
    node->right = NULL;
    node->left = NULL;
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

t_token *find_highest_node(t_token *token)
{
    t_token *token;
    t_token *highest;

    highest = NULL;
    while(token->next != NULL)
    {
        if (ft_strncmp(token->str, ';', 1) == 0)
        {
            highest = token;
        }
        else if ()
        token = token->next;
    }
    return token;
}
// 色んな関数のまとめ？
void parser(t_token *token)
{
    t_token *current;
    current = find_highest_node(token);
    while()
    
}