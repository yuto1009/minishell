/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:33:09 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/07 14:15:58 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int unsupported_variable(t_token *token)
{
    if(token->kind == TK_OPERATOR)
    {
        if(ft_strncmp(token->str , "||",2) == 0)
            return unsupported_token_msg("||");
        else if(ft_strncmp(token->str , ";",1) == 0)
            return unsupported_token_msg(";");
        else if(ft_strncmp(token->str , "&&",2) == 0)
            return unsupported_token_msg("&&");
        else if(ft_strncmp(token->str , "(",1) == 0 || ft_strncmp(token->str , ")",1) == 0)
            return unsupported_token_msg("()");
        else if(ft_strncmp(token->str , "&",1) == 0)
            return unsupported_token_msg("&");
    }

    // unsupported = { "||", "&&", "&", ";", ";;", "(", ")"};
    return 0;
}


int tokenize_error(t_token *token)
{
    if(token == NULL)
    return 0;
    if(token->kind ==TK_OPERATOR)
        return syntax_error_exit(token->str);
    if((token->kind == TK_REDIRECTION)&& count_token_len(token) == 1)
        return  syntax_error_exit("newline");

    while(token->kind!= TK_EOF && token->next->kind!=TK_EOF)
    {
        // if(unsupported_variable(token->next) < 0)
        //     return 127;
        if(token->kind == TK_OPERATOR && token->next->kind == TK_OPERATOR)
            return syntax_error_exit(token->next->str);            
        else if(token->kind == TK_REDIRECTION && token->next->kind == TK_REDIRECTION)
            return syntax_error_exit(token->next->str);
        else if(token->kind == TK_REDIRECTION && token->next->kind == TK_OPERATOR)
            return syntax_error_exit(token->next->str);
        token = token->next;
    }
    if((token->kind == TK_REDIRECTION)&&token->next->kind == TK_EOF)
        return syntax_error_exit("newline");
    else if((token->kind == TK_OPERATOR)&&token->next->kind == TK_EOF)
        return syntax_error_exit(token->str);
    return 0;
}
