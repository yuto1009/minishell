/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/03/23 23:31:13 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
static void append_variable(char **str, char **new_str, t_var *env_map, int prev_status);
// int g_status = 0;
static void append_char(char **str, char new_char)
{
    size_t len;
    char *new_str;

    len = 2;
    if (*str)
        len += ft_strlen(*str);
    new_str = (char *)ft_calloc(len, sizeof(char));
    if (new_str == NULL)
        fatal_error("Malloc Error");
    if (*str)
        ft_strlcpy(new_str, *str, len);
    new_str[len - 2] = new_char;
    if (*str)
        free(*str);
    *str = new_str;
}
// 文字列の入ってない（null）TK_WORDトークンを除去r
static void remove_void_tokens(t_token *token)
{
    if (token->kind == TK_EOF)
        return ;
    if (token->kind == TK_WORD && token->str == NULL)
    {
        t_token *void_token;
        
        void_token = token;
        token = token->prev;
        token->next = token->next->next;
        free(void_token);
    }
    return (remove_void_tokens(token->next));
}

static void remove_single_quote(char **str, char **new_str)
{
    if (**str == SINGLE_QUOTE)
    {
        (*str)++;
        while(**str != SINGLE_QUOTE)
        {
            if (**str == '\0')
            {
                minishell_error("Unclosed single qupte");
            }
            append_char(new_str, **str);
            (*str)++;
        }
        (*str)++;
        return ;
    } 
    fatal_error("Expected single quote");
}

static void remove_double_quote(char **str,char **new_str)
{
    if (**str == DOUBLE_QUOTE)
    {
        (*str)++;
        while(**str != DOUBLE_QUOTE)
        {
            if (**str == '\0')
            {
                minishell_error("Unclosed double qupte");
                break;
            }
            append_char(new_str, **str);
            (*str)++;
        }
        (*str)++;
        return ;
    } 
    fatal_error("Expected double quote");
}
// クオートの削除
static void remove_quotes(t_token *token)
{
    char *str = token->str;
    char *new_str;

    if (token == NULL || token->str == NULL) //delete token->kind != TK_WORD because TK_REDIRECTION
        return ;
    new_str = (char *)ft_calloc(1, sizeof(char));
    if (new_str == NULL)
        fatal_error("Malloc Error");
    
    // シングルとダブルクオートを削除
    while(*str != '\0')
    {
        if(*str == SINGLE_QUOTE)
            remove_single_quote(&str, &new_str);
        else if (*str == DOUBLE_QUOTE)
            remove_double_quote(&str, &new_str);
        else
        {
            append_char(&new_str, *str);
            str++;
        }
    }    
    free(token->str);
    token->str = new_str;
    remove_quotes(token->next);
}

static void append_single_quote(char **str, char **new_str)
{
    if (**str == SINGLE_QUOTE)
    {
        append_char(new_str ,**str);
        (*str)++;
        while (**str != SINGLE_QUOTE)
        {
            if (**str == '\0')
            {
                minishell_error("Unclosed single quote");
                g_status = MINISHELL_ERROR;
                return;
            }
            append_char(new_str, **str);
            (*str)++;
        }
        append_char(new_str, **str);
        (*str)++;
        return;
    }
    fatal_error("Expected single quote");
}
static bool is_dollar_sign(char c)
{
    return (c == '$');
}

static void append_double_quote(char **str, char **new_str, t_var *env_map,int prev_status)
{
    if (**str == DOUBLE_QUOTE)
    {
        append_char(new_str ,**str);
        (*str)++;
        while (**str != DOUBLE_QUOTE )
        {
            if (**str == '\0')
            {
                minishell_error("Unclosed double quote");
                g_status = MINISHELL_ERROR;
                return;
            }
            else if (is_dollar_sign(**str))
                append_variable(str, new_str, env_map,prev_status);
            else
            {
                append_char(new_str, **str);
                (*str)++;
            }
        }
        append_char(new_str, **str);
        (*str)++;
        return;
    }
    fatal_error("Expected double quote");
}


// 単体のドルサインか確認
static bool is_single_dollar_sign(char *str)
{
    char *p;
    p = str;
    if (*p == DOLLAR_SIGN)
    {
        p++;
        if (*p == '\0' || *p == ' ' ||  *p =='\"' || *p == '\"')
            return true;
    }
    return false;
}

// 単体のドルサインか確認
static bool is_env_variable(char *str)
{
    char *p;
    p = str;
    if (*p == DOLLAR_SIGN)
    {
        p++;
        if (*p >= 'A' && *p <='Z')
            return true;
    }
    return false;
}
static bool is_alpha(char chr)
{
    return (('a'<=chr && chr<='z') || ('A'<= chr && chr<='Z'));
}
static void append_env_variable(char **str, char **new_str, t_var *env_map)
{
   
    char *env_name = ft_calloc(1,sizeof(char));
    char *ans;
    while(**str !='\0'&&is_alpha(**str))
    {
        append_char(&env_name, **str);
        (*str)++;
        // printf("str: %s\n",*str);
    }
    ans = get_env_value(env_name,env_map);
    free(env_name);
    if(!ans)
        *new_str =NULL;
    // getenvがnullを返してきた時は何もnew_strにappendしない strだけはその変数分インクリメント
    if(ans)
    {
        while(*ans)
        {
            append_char(new_str, *ans);
            ans++;
        }
    }
}

static bool is_exit_status(char *str)
{
    char *p;
    p = str;
    if (*p == DOLLAR_SIGN)
    {
        p++;
        if (*p == QUESTION)
            return true;
    }
    return false;
}

static void append_question(char **str, char **new_str,int prev_status)
{
    char *exit_status;
    (*str)++;   // ?をインクリメント
    exit_status = ft_itoa(prev_status);
    if (exit_status == NULL)
        fatal_error("MALLOC ERROR");
    while(*exit_status)
    {
        append_char(new_str, *exit_status);
        exit_status++;
    }
}


static void append_variable(char **str, char **new_str, t_var *env_map, int prev_status)
{
    // ドルサイン単体であれば、そのままドルサインを返す
    if (is_single_dollar_sign(*str)){
        append_char(new_str, **str);
        (*str)++;

    }
    else if (is_env_variable(*str))  // ドルサイン+大文字があったらappend_env_variable
    {
        (*str)++;
        append_env_variable(str, new_str, env_map);
    }
    else if (is_exit_status(*str))
    {
        // puts("Debug");
        (*str)++;
        append_question(str, new_str,prev_status);
    }
    else{
        while(**str!='\0')
            (*str)++;
        return;
    }

    // else minishell_error
}

// 文字列に文字を追加する

// 変数展開（シェル変数・環境変数）
// 各トークンを再帰的に巡る
// 一文字ずつクオート・変数を確認
static void expand_variable(t_token *token, t_var *env_map,int prev_status)
{
    char *new_str;
    char *str = token->str;
 
    if (token == NULL || token->kind == TK_EOF)
        return;
    if (token->kind != TK_WORD || token->str == NULL)
        return expand_variable(token->next, env_map,prev_status);
    new_str = (char *)calloc(1, sizeof(char));
    if (new_str == NULL)
        fatal_error("Malloc Error");
    while(*str != '\0')
    {
        if (*str == SINGLE_QUOTE)
            append_single_quote(&str, &new_str);
        else if (*str == DOUBLE_QUOTE)
            append_double_quote(&str, &new_str, env_map,prev_status);
        else if (is_dollar_sign(*str) == true)
            append_variable(&str, &new_str, env_map,prev_status);
        else
        {
            append_char(&new_str, *str);
            str++;
        }
    }
        if(g_status == MINISHELL_ERROR)
            return ;
    free(token->str);
    token->str = new_str;

    expand_variable(token->next, env_map,prev_status);
}

int expand(t_token *token, t_var *env_map, int prev_status)
{
    // 変数展開とクオートの削除
    // t_token *first_token = token;
    expand_variable(token, env_map,prev_status);
    if(g_status == MINISHELL_ERROR)
        return -1;
    // print_tokens(token);
    if(token->str == NULL)
     {
        g_status = 0;
        return -1;
     }   
    remove_quotes(token);
    // TK_WORD && str == NULL -> tokenなかったことにする
    // ここに作る
    remove_void_tokens(token);
    // token = first_token;
    return 0;
}