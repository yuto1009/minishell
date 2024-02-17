/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/02/17 18:50:43 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool contains_dollar_sign(char *str)
{
    // strにドルサインが入ってたらtrueを返す（例外なし）
}

static char *replace_env_with_expanded_env(char *str, char *env, char *expanded_env)
{
    const size_t new_str_len = ft_strlen(str) - ft_strlen(env) + ft_strlen(expanded_env); 
    char *new_str = (char *)calloc(new_str_len, sizeof(char *));
    if (new_str == NULL)
        fatal_error("Malloc Error");
    
    // 古い文字列の解放忘れずに
}

static void expand_dollar_sign(char *str, char *env)
{
    const char *expanded_env = getenv(env);
    
    if (expanded_env == NULL)
        return str;
    replace_env_with_expanded_env(str, env, expanded_env);
}

static void expand_token(t_token *token)
{
    const char *str = token->str;
    
    while (*str != '\0')
    {
        if (*str == SINGLE_QUOTE)
        {
            while (*str != '\0' && *str != SINGLE_QUOTE)
                str++;
            if (*str == '\0')
                return ;
        }
        if (*str == DOLLAR_SIGN && str+1 == '\0')
        {
            return;
        }
        else if (*str == DOLLAR_SIGN)
        {
            expand_dollar_sign(token->str, str);
        }
        str++;
    }
}

static void remove_quotes(char **str)
{
    // シングルクオートとダブルクオートをbashの挙動の通りに除去
    // tokeninze.cのtokenize_word関数にコードあり
}

void expand(t_node *node)
{
    // nodeを昇る
    while(node != NULL)
    {   
        // tokenを昇る
        while(node->token->kind !=TK_EOF)
        {
            // tokenがWORD && ドルサインがあれば、展開に進む
            if(node->token->kind == TK_WORD && contains_dollar_sign(node->token->str))
            {
                expand_token(node->token);
            }
            // トークンからクオートを除去
            remove_quotes(node->token);
            node->token = node->token->next;
        }
        node = get_next_node(node);
    }
}