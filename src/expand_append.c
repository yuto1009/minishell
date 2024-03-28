/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 12:39:38 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/28 16:57:27 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		while (**str != '\0')
			(*str)++;
		return ;
	}
}
