/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 12:40:30 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/03 22:48:23 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_char(char **str, char new_char)
{
	size_t	len;
	char	*new_str;

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

void	append_single_quote(char **str, char **new_str)
{
	if (**str == SINGLE_QUOTE)
	{
		append_char(new_str, **str);
		(*str)++;
		while (**str != SINGLE_QUOTE)
		{
			if (**str == '\0')
			{
				minishell_error("Unclosed single quote");
				free(*new_str);
				g_status = MINISHELL_ERROR;
				return ;
			}
			append_char(new_str, **str);
			(*str)++;
		}
		append_char(new_str, **str);
		(*str)++;
		return ;
	}
	fatal_error("Expected single quote");
}

static void	process_double_quote_contents(char **str, char **new_str,
		t_var *env_map, int prev_status)
{
	while (**str != DOUBLE_QUOTE)
	{
		if (**str == '\0')
		{
			minishell_error("Unclosed double quote");
			free(*new_str);
			g_status = MINISHELL_ERROR;
			return ;
		}
		else if (is_dollar_sign(**str))
		{
			append_variable(str, new_str, env_map, prev_status);
		}
		else
		{
			append_char(new_str, **str);
			(*str)++;
		}
	}
	append_char(new_str, **str);
	(*str)++;
}

void	append_double_quote(char **str, char **new_str, t_var *env_map,
		int prev_status)
{
	if (**str == DOUBLE_QUOTE)
	{
		append_char(new_str, **str);
		(*str)++;
		process_double_quote_contents(str, new_str, env_map, prev_status);
	}
	else
	{
		fatal_error("Expected double quote");
	}
}

void	append_question(char **str, char **new_str, int prev_status)
{
	char	*exit_status;

	(*str)++;
	exit_status = ft_itoa(prev_status);
	if (exit_status == NULL)
		fatal_error("MALLOC ERROR");
	while (*exit_status)
	{
		append_char(new_str, *exit_status);
		free(exit_status);
		exit_status++;
	}
}
