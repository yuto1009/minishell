/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:33:09 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/28 16:10:51 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	tokenize_error(t_token *token)
{
	if (token == NULL || token->str == NULL)
		return (0);
	if (token->kind == TK_OPERATOR)
		return (syntax_error_exit(token->str));
	if ((token->kind == TK_REDIRECTION) && count_token_len(token) == 1)
		return (syntax_error_exit("newline"));
	while (token->kind != TK_EOF && token->next->kind != TK_EOF)
	{
		if (token->kind == TK_OPERATOR && token->next->kind == TK_OPERATOR)
			return (syntax_error_exit(token->next->str));
		else if (token->kind == TK_REDIRECTION
			&& token->next->kind == TK_REDIRECTION)
			return (syntax_error_exit(token->next->str));
		else if (token->kind == TK_REDIRECTION
			&& token->next->kind == TK_OPERATOR)
			return (syntax_error_exit(token->next->str));
		token = token->next;
	}
	if ((token->kind == TK_REDIRECTION) && token->next->kind == TK_EOF)
		return (syntax_error_exit("newline"));
	else if ((token->kind == TK_OPERATOR) && token->next->kind == TK_EOF)
		return (syntax_error_exit(token->str));
	return (0);
}
