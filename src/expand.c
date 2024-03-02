/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/03/02 19:21:22 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static size_t count_val_len(const char *str)
{
    size_t len;

    len = 0;
    while (ft_isalpha(str[len]) == true || str[len] == '_' || str[len] == '?')
        len++;
    return (len);
}

static char *expand_dollar_sign(const char *str)
{
    const size_t env_len = count_val_len(str + 1);
    char *env;
    
    env = ft_substr(str, 1, env_len);
    const char *expanded_env = getenv(env);
    if (expanded_env == NULL)
    {
        return NULL;
    }
    free(env);  
    str = str + env_len + 1;
    const char *expanded_str = ft_strjoin(expanded_env, str);
    return ((char *)expanded_str);
}

static void expand_token(t_token *token)
{
    const char *str = token->str;
    size_t i;

    i = 0;    
    while (str[i] != '\0')
    {
        if (str[i] == SINGLE_QUOTE)
        {
            while (str[i] != '\0' && str[i] != SINGLE_QUOTE)
                i++;
            if (str[i] == '\0')
                return ;
        }
        else if (str[i] == DOLLAR_SIGN && str[i+1] == '\0')
        {
            return;
        }
        else if (str[i] == DOLLAR_SIGN)
        {
            char *expanded_str = ft_substr(token->str, 0, i);
            char *newly_expanded_str = expand_dollar_sign(&str[i]);
            free(token->str);
            token->str = ft_strjoin(expanded_str, newly_expanded_str);
            free(expanded_str);
            free(newly_expanded_str);
            str += i;
            continue;
        }
        i++;
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

void expand(t_token *token)
{
    // tokenを昇る
    t_token *tmp_token;
    tmp_token = token;
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
                if (tmp_token->prev != NULL)
                    tmp_token->prev->next = tmp_token->next;
                else
                    token = tmp_token->next;
                free((void *)void_token);
                continue;
            }
        }
        // トークンからクオートを除去
        tmp_token->str = remove_single_quotes(&tmp_token->str);
        tmp_token = tmp_token->next;
    }
}