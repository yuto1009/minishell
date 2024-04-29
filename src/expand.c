/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 16:00:29 by yuendo            #+#    #+#             */
/*   Updated: 2024/04/29 10:39:34 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	expand_loop(char **str, char **new_str, t_var *env_map,
		int prev_status)
{
	while (**str != '\0')
	{
		if (**str == SINGLE_QUOTE)
			append_single_quote(str, new_str);
		else if (**str == DOUBLE_QUOTE)
			append_double_quote(str, new_str, env_map, prev_status);
		else if (is_dollar_sign(**str) == true)
			append_variable(str, new_str, env_map, prev_status);
		else
		{
			append_char(new_str, **str);
			(*str)++;
		}
	}
}

static void	expand_variable(t_token *token, t_var *env_map, int prev_status)
{
	char	*new_str;
	char	*str;

	str = token->str;
	if (token == NULL || token->kind == TK_EOF)
		return ;
	if (token->kind != TK_WORD || token->str == NULL)
		return (expand_variable(token->next, env_map, prev_status));
	new_str = (char *)ft_calloc(1, sizeof(char));
	if (new_str == NULL)
		fatal_error("Malloc Error");
	expand_loop(&str, &new_str, env_map, prev_status);
	if (g_status == MINISHELL_ERROR)
		return ;
	free(token->str);
	token->str = new_str;

	expand_variable(token->next, env_map, prev_status);
}

int	expand(t_token *token, t_var *env_map, int prev_status)
{
	expand_variable(token, env_map, prev_status);
	if (g_status == MINISHELL_ERROR)
		return (-1);
	if (token->str == NULL)
	{
		g_status = 0;
		return (-1);
	}
	remove_quotes(token);
	remove_void_tokens(token);
	return (0);
}
