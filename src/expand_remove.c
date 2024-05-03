/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 12:44:38 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/03 22:00:40 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	remove_void_tokens(t_token *token)
{
	t_token	*void_token;

	if (token->kind == TK_EOF)
		return ;
	if (token->kind == TK_WORD && token->str == NULL)
	{
		void_token = token;
		token = token->prev;
		token->next = token->next->next;
		free(void_token);
	}
	return (remove_void_tokens(token->next));
}

void	remove_single_quote(char **str, char **new_str)
{
	if (**str == SINGLE_QUOTE)
	{
		(*str)++;
		while (**str != SINGLE_QUOTE)
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

void	remove_double_quote(char **str, char **new_str)
{
	if (**str == DOUBLE_QUOTE)
	{
		(*str)++;
		while (**str != DOUBLE_QUOTE)
		{
			if (**str == '\0')
			{
				minishell_error("Unclosed double qupte");
				break ;
			}
			append_char(new_str, **str);
			(*str)++;
		}
		(*str)++;
		return ;
	}
	fatal_error("Expected double quote");
}

void	remove_quotes(t_token *token)
{
	char	*str;
	char	*new_str;

	str = token->str;
	if (token == NULL || token->str == NULL)
		return ;
	new_str = (char *)ft_calloc(1, sizeof(char));
	if (new_str == NULL)
		fatal_error("Malloc Error");
	while (*str != '\0')
	{
		if (*str == SINGLE_QUOTE)
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
