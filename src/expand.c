/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/02/24 12:03:57 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static char *replace_env_with_expanded_env(char *str, char *env, char *expanded_env)
// {
//     const size_t new_str_len = ft_strlen(str) - ft_strlen(env) + ft_strlen(expanded_env);
//     char *new_str = (char *)calloc(new_str_len, sizeof(char *));
//     if (new_str == NULL)
//         fatal_error("Malloc Error");
    
//     // 古い文字列の解放忘れずに
//     ft_strlcat(new_str, expanded_env, new_str_len);
//     ft_strlcat(new_str, env + ft_strlen(env), new_str_len);
//     return (new_str);
// }

// static char *retrieve_expanded_variable(char *env)
// {
//     const char *expanded_env = getenv(env);
//     return (expanded_env);
// }

static char *expand_dollar_sign(const char *str, const char *env)
{
    const char *expanded_env = getenv(env);
    const size_t new_str_len = ft_strlen(str) - ft_strlen(env) + ft_strlen(expanded_env);
    char *new_str = (char *)calloc(new_str_len, sizeof(char *));
    if (new_str == NULL)
        fatal_error("Malloc Error");
    
    // 古い文字列の解放忘れずに
    ft_strlcat(new_str, expanded_env, new_str_len);
    ft_strlcat(new_str, env + ft_strlen(env), new_str_len);
    return (new_str);
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
        if (*str == DOLLAR_SIGN && *(str+1) == '\0')
        {
            return;
        }
        else if (*str == DOLLAR_SIGN)
        {
            const char *expanded_token = (char *)ft_calloc(1, sizeof(char *));
            if (expanded_token == NULL)
                fatal_error("Malloc Error");
            expanded_token = expand_dollar_sign(token->str, str);
            free(token->str);
            token->str = (char *)expanded_token;
        }
        str++;
    }
}

static char *remove_quotes(char **str)
{
    // シングルクオートとダブルクオートをbashの挙動の通りに除去
    // tokeninze.cのtokenize_word関数にコードあり
    const size_t word_size = ft_strlen(*str) + 1;
    char *word;
    size_t i;
    size_t j;

    word = (char *)ft_calloc(word_size, sizeof(char));
    if (word == NULL)
        fatal_error("malloc error");
    i = 0;
    j = 0;
    while ((*str)[i] != '\0' && is_metacharacter((*str)[i]) == false)
    {
        if ((*str)[i] == SINGLE_QUOTE || (*str)[i] == DOUBLE_QUOTE)
        {
            const char current_quote = (*str)[i];
            i++;    // クオートをスキップ
            while ((*str)[i] != '\0' && (*str)[i] != current_quote)
            {
                word[j] = (*str)[i];
                i++;
                j++;
            }
            if ((*str)[i] == '\0')
            {
                free(word);
                minishell_error("unclosed quote");
            }
            i++;    // 閉じクオートスキップ
        }
        else
        {
            word[j] = (*str)[i];
            i++;
            j++;
        }
    }
    word[j] = '\0';
    return (word);
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
            if(node->token->kind == TK_WORD && ft_strchr(node->token->str, DOLLAR_SIGN))
            {
                expand_token(node->token);
            }
            // トークンからクオートを除去
            const char *trimmed_str = remove_quotes(&node->token->str);
            free(node->token->str);
            node->token->str = (char *)trimmed_str;
            node->token = node->token->next;
        }
        node = get_next_node(node);
    }
}