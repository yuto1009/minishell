/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 12:39:38 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/05 22:09:30 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_lower_case(char *str)
{
	return ('a' <= *str && *str <= 'z');
}

static void	handle_quotes_and_end(char **str)
{
	(*str)++;
	if (ft_strchr(*str, '"') != NULL)
	{
		while (**str != '\"')
			(*str)++;
	}
	else
	{
		while (**str != '\0')
			(*str)++;
	}
}

void	append_variable(char **str, char **new_str, t_var *env_map,
		int prev_status)
{
	if (is_single_dollar_sign(*str))
	{
		append_char(new_str, **str);
		(*str)++;
	}
	else if (is_env_variable(*str))
	{
		(*str)++;
		append_env_variable(str, new_str, env_map);
	}
	else if (is_exit_status(*str))
	{
		(*str)++;
		append_question(str, new_str, prev_status);
	}
	else
	{
		handle_quotes_and_end(str);
	}
}
