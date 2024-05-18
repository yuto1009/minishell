/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:30:50 by kyoshida          #+#    #+#             */
/*   Updated: 2024/05/18 14:42:28 by kyoshida         ###   ########.fr       */
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
	if (args[1] != NULL && ft_strncmp(args[1], "PWD", 3) == 0)
		return (0);
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
