/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:37:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/03 22:37:45 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_env_variable(char **str, char **new_str, t_var *env_map)
{
	char	*env_name;
	char	*ans;
	char	*head;

	env_name = ft_calloc(1, sizeof(char));
	while (**str != '\0' && is_alpha(**str))
	{
		append_char(&env_name, **str);
		(*str)++;
	}
	ans = get_env_value(env_name, env_map);
	head = ans;
	free(env_name);
	if (!ans)
	{
		free(*new_str);
		*new_str = NULL;
	}
	if (ans)
	{
		while (*ans)
		{
			append_char(new_str, *ans);
			ans++;
		}
		free(head);
	}
}
