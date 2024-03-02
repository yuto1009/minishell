/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/03/02 13:02:18 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *expand_dollar_sign(char *str, const char *env)
{
    printf("str: %s\n", str);
    printf("env: %s\n", env);
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

static char *remove_double_quotes(char **str)
{
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
        if ((*str)[i] == DOUBLE_QUOTE)
        {
            i++;    // クオートをスキップ
            while ((*str)[i] != '\0' && (*str)[i] != DOUBLE_QUOTE)
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
    free(*str);
    return (word);
}

static char *remove_single_quotes(char **str)
{
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
        if ((*str)[i] == SINGLE_QUOTE)
        {
            i++;    // クオートをスキップ
            while ((*str)[i] != '\0' && (*str)[i] != SINGLE_QUOTE)
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
    free(*str);
    return (word);
}

// static char *remove_quotes(char **str)
// {
//     // シングルクオートとダブルクオートをbashの挙動の通りに除去
//     // tokeninze.cのtokenize_word関数にコードあり
//     const size_t word_size = ft_strlen(*str) + 1;
//     char *word;
//     size_t i;
//     size_t j;

//     word = (char *)ft_calloc(word_size, sizeof(char));
//     if (word == NULL)
//         fatal_error("malloc error");
//     i = 0;
//     j = 0;
//     while ((*str)[i] != '\0' && is_metacharacter((*str)[i]) == false)
//     {
//         if ((*str)[i] == SINGLE_QUOTE || (*str)[i] == DOUBLE_QUOTE)
//         {
//             const char current_quote = (*str)[i];
//             i++;    // クオートをスキップ
//             while ((*str)[i] != '\0' && (*str)[i] != current_quote)
//             {
//                 word[j] = (*str)[i];
//                 i++;
//                 j++;
//             }
//             if ((*str)[i] == '\0')
//             {
//                 free(word);
//                 minishell_error("unclosed quote");
//             }
//             i++;    // 閉じクオートスキップ
//         }
//         else
//         {
//             word[j] = (*str)[i];
//             i++;
//             j++;
//         }
//     }
//     word[j] = '\0';
//     return (word);
// }

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
        t_token *tmp_token;
        tmp_token = node->token;
        while(tmp_token != NULL && tmp_token->kind !=TK_EOF)
        {
            tmp_token->str = remove_double_quotes(&tmp_token->str);
            // tokenがWORD && ドルサインがあれば、展開に進む
            if(tmp_token->kind == TK_WORD && ft_strchr(tmp_token->str, DOLLAR_SIGN))
            {
                expand_token(tmp_token);  // トークンを展開へ
                if (is_unknown_env(tmp_token) == true)    // 指定の環境変数が存在しない場合、そのトークンはなかったことにする
                {
                    const t_token *void_token = tmp_token;
                    if (tmp_token->prev != NULL && tmp_token->next != NULL)
                        tmp_token->prev->next = tmp_token->next;
                    tmp_token = tmp_token->next; 
                    free((void *)void_token);
                    continue;
                }
            }
            // トークンからクオートを除去
            printf("token->str: %s\n", tmp_token->str);
            tmp_token->str = remove_single_quotes(&tmp_token->str);
            tmp_token = tmp_token->next;
        }
        node = node->next;
    }
}