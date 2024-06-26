/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:37:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/05 22:12:33 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	extract_env_variable_name(char **str, char **env_name)
{
	*env_name = ft_calloc(1, sizeof(char));
	while (**str != '\0' && is_alpha(**str))
	{
		append_char(env_name, **str);
		(*str)++;
	}
}

static void	process_env_variable_value(char *ans, char **new_str)
{
	char	*head;

	head = ans;
	if (!ans)
		return ;
	else
	{
		while (*ans)
		{
			append_char(new_str, *ans);
			ans++;
		}
		free(head);
	}
}

void	append_env_variable(char **str, char **new_str, t_var *env_map)
{
	char	*env_name;
	char	*ans;

	extract_env_variable_name(str, &env_name);
	ans = get_env_value(env_name, env_map);
	process_env_variable_value(ans, new_str);
	free(env_name);
}
