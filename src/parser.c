/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/01/14 22:03:25 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

typedef struct s_token t_token;
typedef struct s_node t_node;

struct s_node {
    t_token *token;
    t_node *left;
    t_node *right;
};

// 見つけた最高層のノードを軸に子ノードを格納する
t_node *new_node(t_token *axis, t_token *left_token, t_token *right_token)
{
    t_node *node;

    node = (t_node *)ft_calloc(1, sizeof(t_node));
    if (node == NULL)
        fatal_error("malloc error");
    node->token = axis;
    if (left_token != NULL)
        node->left = new_node(left_token, NULL, NULL);
    else
        node->left = NULL;
    if (right_token != NULL)
        node->right = new_node(right_token, NULL, NULL);
    else
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
    if(token->next->kind == TK_EOF)
        return (new_token(NULL, TK_EOF));   // もし最高層ノードの右側にトークンがなかったらEOFトークンだけ入れる
    token = token->next;
    return (token);
}

t_token *find_axis_token(t_token *token)
{
    t_token *axis;

    axis = NULL;
    while(token->next != NULL)
    {
        if (ft_strncmp(token->str, ";", 1) == 0)    // このif文続きありそう
            axis = token;
        else if (ft_strncmp(axis->str, ";", 1) != 0 && ft_strncmp(token->str , "|", 1) == 0)
            axis = token;
        token = token->next;
    }
    return axis;
}

// 色んな関数のまとめ？
void parser(t_node *node)
{
    t_token *axis_token;
    t_token *left_token;
    t_token *right_token;
    
    axis_token = find_axis_token(node->token);
    if(axis_token == NULL)
        return;
    right_token = split_right_tokens(axis_token);
    left_token = split_left_tokens(axis_token);
    node = new_node(axis_token, right_token, left_token);
    // parser(node->left);
    // parser(node->right);
}


int main(void)
{
    t_node *node;
    node->token->str = 
    node->token->kind = 
   parser(node);

}