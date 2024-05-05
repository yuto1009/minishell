/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 12:39:38 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/05 17:27:06 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool is_lower_case(char *str)
{
    return ('a'<= *str && *str <= 'z');
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
        (*str)++;
        if(ft_strchr(*str,'"') != NULL)
        {
           while(**str!='\"')
           (*str)++;
           return;
        }
        
		while (**str != '\0')
			(*str)++;
		return ;
	}
}
