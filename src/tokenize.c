/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 18:57:26 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/23 17:03:58 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool is_operator(char *line)
{
    const char *operators[] = { "||", "&&", "&", ";", ";;", "(", ")", "|", "\n"};
    size_t i = 0;
    
    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (ft_strncmp(line, operators[i], ft_strlen(operators[i])) == 0) 
        {
            return (true);
        }
        i++;
    }
    return (false);
}

static bool is_redirection_operator(char *line)
{
    const char *operators[] = {">>","<<","<>","<",">"};

    size_t i = 0;
    while (i < sizeof(operators) / sizeof(*operators))//comment by kyoshida iを配列の要素数より小さいだけ回している？
    {
        if (ft_strncmp(line, operators[i], ft_strlen(operators[i])) == 0) 
        {
            return (true);
        }
        i++;
    }
    return (false);

}
static int is_blank(char c) 
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static bool is_metacharacter(char c)
{
    const char *metacharacters = "| \t";

    if (ft_strchr(metacharacters, c) != NULL)
    {
        return (true);
    }
    return (false);
}

static t_token *new_token(char *str, token_kind kind)
{
    t_token *token;

    token = (t_token *)ft_calloc(1, sizeof(t_token));
    if (token == NULL)
        fatal_error("malloc error");
    token->str = str;
    token->kind = kind;
    token->prev = NULL;
    token->next = NULL;
    return (token);
}

static t_token *tokenize_operator(char **line)
{
    const char *operators[] = {"||", "&&", ";;", "&", ";", "(", ")", "|", "\n"};
    char *operator;
    size_t i;

    operator = NULL;
    i = 0;
    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (ft_strncmp(*line, operators[i], ft_strlen(operators[i])) == 0) 
        {
            break;
        }
        i++;
    }
    operator = ft_substr(*line, 0, ft_strlen(operators[i]));
    if (operator == NULL)
        fatal_error("malloc error");
    *line += ft_strlen(operator);
    return (new_token(operator, TK_OPERATOR));
} 

static t_token *tokenize_redirection_operator(char **line)
{
    const char *operators[] = {">>","<<","<>","<",">"};
    char *operator;
    size_t i;

    operator = NULL;
    i = 0;
    while (i < sizeof(operators) / sizeof(*operators))
    {
        if (ft_strncmp(*line, operators[i], ft_strlen(operators[i])) == 0) 
        {
            break;
        }
        i++;
    }
    operator = ft_substr(*line, 0, ft_strlen(operators[i]));
    if (operator == NULL)
        fatal_error("malloc error");
    *line += ft_strlen(operator);
    return (new_token(operator, TK_REDIRECTION));
}
static bool is_metacharacter_token(char c)
{
    const char *metacharacters = "|\t";
    
    if(c== '\0')
    return false;
    if (ft_strchr(metacharacters, c) != NULL)
    {
        return (true);
    }
    return (false);
}
static bool is_multquote(char *line)
{
    int i =0;
    while(line[i]!='\0')
    {
        if(line[i] == DOUBLE_QUOTE && line[i+1] == DOUBLE_QUOTE)
            return true;
        else if(line[i] == SINGLE_QUOTE && line[i+1] == SINGLE_QUOTE)
            return true;
        i++;
    }
    return false;
}
static t_token *tokenize_word(char **line) 
{
    const size_t word_size = ft_strlen(*line) + 1;
    char *word;
    size_t i;
    size_t j;
    int tmp;
    word = (char *)ft_calloc(word_size, sizeof(char));
    if (word == NULL)
        fatal_error("malloc error");
    i = 0;
    tmp = 0;
    j = 0;
    while ((*line)[i] != '\0' && is_metacharacter((*line)[i]) == false)
    {
        if ((*line)[i] == SINGLE_QUOTE || (*line)[i] == DOUBLE_QUOTE)
        {
            const char current_quote = (*line)[i];
            while((*line)[i] != '\0'&&!is_metacharacter_token((*line)[i]))
            {
                if((*line)[i] == current_quote&&(((*line)[i+1] == '\0')||is_metacharacter((*line)[i+1])))
                {
                    tmp = i+1;
                    while(is_blank((*line)[tmp]))
                        tmp++;
                    if(!((*line)[tmp] == DOUBLE_QUOTE || (*line)[tmp] == SINGLE_QUOTE) ||!is_multquote(*line)){
                        word[j] = (*line)[i];
                        j++;
                        break;
                    }
                }
                word[j] = (*line)[i];
                i++;
                j++;
            }
            if((*line)[i+1] == '\0'||is_metacharacter((*line)[i+1])){
                i++;
            break;
            }
            // // i++;    // クオートをスキップ
            // while ((*line)[i] != '\0' && (*line)[i] != current_quote )
            // {
            //     word[j] = (*line)[i];
            //     i++;
            //     j++;
            // }
            // if ((*line)[i] == '\0')
            // {
            //     free(word);
            //     minishell_error("unclosed quote");
            // }
            // i++;    // 閉じクオートスキップ
        }
        else
        {
            word[j] = (*line)[i];
            i++;
            j++;
        }
    }
    word[j] = '\0';
    *line += i;   // 入力から得た文字列をインクリメント
    return (new_token(word, TK_WORD));
}

t_token *tokenize(char *line)
{
    t_token *head;
    t_token *current;
    t_token *new;

    head = NULL;
    current = NULL;
    new = NULL;
    while (*line != '\0')
{
    if (is_blank(*line) == true) {
        line++;
        continue;  // 空白文字の後、次のトークンへ進む
    }
    if (is_operator(line)) {
        new = tokenize_operator(&line);
    }
    else if(is_redirection_operator(line)){
        new = tokenize_redirection_operator(&line);
    }
    else{
        new = tokenize_word(&line);
    }
    
    if (new != NULL) {  // ここでnewがNULLでないことを確認
        if (head == NULL) {
            head = new;
            head->prev = new_token(NULL,TK_EOF);  // 新しいヘッドのprevをNULLに設定
            current = new;
        } else {
            current->next = new;
            new->prev = current;
            current = new;
        }
    }
}

    if (head == NULL)
        head = new_token(NULL, TK_EOF);
    else
    {
        current->next = new_token(NULL, TK_EOF);
        current->next->prev = current;
    }
    return (head);
}
