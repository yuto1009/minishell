/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:30:50 by kyoshida          #+#    #+#             */
/*   Updated: 2024/05/27 20:13:09 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

static bool	is_identifier(char *name)
{
	if (name == NULL || *name == '\0')
		return (false);
	if (!ft_isalpha(*name) && *name != '_')
		return (false);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

int	validate_identifier(char *arg)
{
	if (!is_identifier(arg))
	{
		g_status = GENERAL_ERRORS;
		command_error(arg, "unset");
		return (0);
	}
	return (1);
}

void	process_unset(char *arg, t_var *head)
{
	t_var	*env_map;

	env_map = head;
	while (env_map != NULL)
	{
		if (ft_strncmp(arg, env_map->name, ft_strlen(arg)) == 0)
		{
			unset_env(arg, env_map);
			break ;
		}
		env_map = env_map->next;
	}
}

int	builtin_unset(char **args, t_var *env_map)
{
	int		i;
	t_var	*head;

	i = 1;
	head = env_map;
	while (args[i] != NULL)
	{
		if (!validate_identifier(args[i]))
			return (0);
		process_unset(args[i], head);
		i++;
	}
	return (0);
}
