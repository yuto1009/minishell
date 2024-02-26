/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/02/24 15:19:34 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *expand_dollar_sign(char *str, const char *env)
{
    const char *expanded_env = getenv(env);
    if (expanded_env == NULL)
    {
        free(str);
        return NULL;
    }
    const size_t new_str_len = ft_strlen(str) - ft_strlen(env) + ft_strlen(expanded_env);
    char *new_str = (char *)calloc(new_str_len, sizeof(char *));
    if (new_str == NULL)
        fatal_error("Malloc Error");
    ft_strlcat(new_str, expanded_env, new_str_len);
    ft_strlcat(new_str, env + ft_strlen(env), new_str_len);
    free(str);
    return (new_str);
}

static void expand_token(t_token *token)
{
    const char *str = token->str;
    
    while (str != NULL && *str != '\0')
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
            token->str = expand_dollar_sign(token->str, ++str);
            continue;
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

// getenv関数は未知の環境変数に対してNULLを返す
static bool is_unknown_env(t_token *token)
{
    return (token->str == NULL);
}

void expand(t_node *node)
{
    // nodeを昇る
    while(node != NULL)
    {   
        // tokenを昇る
        t_token *tmp;
        tmp = node->token;
        while(tmp != NULL && tmp->kind !=TK_EOF)
        {
            // tokenがWORD && ドルサインがあれば、展開に進む
            if(tmp->kind == TK_WORD && ft_strchr(tmp->str, DOLLAR_SIGN))
            {
                expand_token(tmp);  // トークンを展開へ
                if (is_unknown_env(tmp) == true)    // 指定の環境変数が存在しない場合、そのトークンはなかったことにする
                {
                    const t_token *void_token = tmp;
                    if (tmp->prev != NULL && tmp->next != NULL)
                        tmp->prev->next = tmp->next;
                    tmp = tmp->next; 
                    free((void *)void_token);
                    continue;
                }
            }
            // トークンからクオートを除去
            const char *trimmed_str = remove_quotes(&tmp->str);
            free(tmp->str);
            tmp->str = (char *)trimmed_str;
            tmp = tmp->next;
        }
        node = node->next;
    }
}