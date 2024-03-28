/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:30:50 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/28 16:41:16 by kyoshida         ###   ########.fr       */
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

int	builtin_unset(char **args, t_var *env_map)
{
	int	i;

	i = 1;
	while (args[i] != NULL)
	{
		while (env_map != NULL)
		{
			if (!is_identifier(args[i]))
			{
				g_status = GENERAL_ERRORS;
				command_error(args[i], "unset");
				return (0);
			}
			if (ft_strncmp(args[i], env_map->name, ft_strlen(args[i])) == 0)
				unset_env(args[i], env_map);
			env_map = env_map->next;
		}
		i++;
	}
	return (0);
}
