/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/01/20 16:43:35 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/minishell.h"
#include "../include/minishell.h"
// typedef struct s_token t_token;
// typedef struct s_node t_node;

// struct s_node {
//     t_token *token;
//     t_node *left;
//     t_node *right;
// };

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
    t_token *new_eof;

    if (token->prev != NULL)
        token = token->prev; // 最高層ノードの左側のトークンに移動する
    new_eof = new_token(NULL, TK_EOF);
    new_eof->prev = token; // 新しいEOFトークンのprevを設定する
    token->next = new_eof;
    // printf("left token: %s\n", token->str);
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


// 色んな関数のまとめ？
t_node *parser(t_node *node)
{
    t_token *axis_token;
    t_token *left_token;
    t_token *right_token;
    
    axis_token = find_axis_token(node->token);
    // if(axis_token == NULL)
    if(axis_token->kind == TK_EOF)
        return NULL;
    printf("axis_token: %s\n", axis_token->str);
    right_token = split_right_tokens(axis_token);
    printf("after split right token: %s\n", right_token->str);
    left_token = split_left_tokens(axis_token);
    printf("after split left token: %s\n", left_token->str);
    node = new_node(axis_token, right_token, left_token);
    // printf("node->str : %s\n",node->token->str);
    // printf("node->right : %s\n",node->right->token->str);
    // printf("node->left : %s\n",node->left->token->str);
    printf("\n");
    parser(node->right);
    parser(node->left);
    return node;
    
}


// int main(void)
// {
//     t_node *node;
//     node->token->str = 
//     node->token->kind = 
//    parser(node);

// }