/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_remove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 12:44:38 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/06/01 18:35:55 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token	*remove_initial_void_token(t_token *token)
{
	t_token	*tmp;

	if (token->prev->kind == TK_EOF && token->str[0] == '\0')
	{
		tmp = token;
		token = token->next;
		token->prev = tmp->prev;
		free(tmp->str);
		free(tmp);
	}
	while (token->kind != TK_EOF && token->str[0] == '\0')
	{
		tmp = token;
		token = token->next;
		token->prev = tmp->prev;
		free(tmp->str);
		free(tmp);
	}
	return (token);
}

t_token	*remove_void_tokens(t_token *token)
{
	t_token	*head;
	t_token	*temp_token;

	head = remove_initial_void_token(token);
	if (head->kind == TK_EOF)
		return (head);
	token = head;
	while (token != NULL && token->kind != TK_EOF)
	{
		if (token->str != NULL && token->str[0] == '\0')
		{
			temp_token = token;
			token->prev->next = token->next;
			if (token->next != NULL)
			{
				token->next->prev = token->prev;
			}
			token = token->next;
			free(temp_token->str);
			free(temp_token);
		}
		else
			token = token->next;
	}
	return (head);
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
