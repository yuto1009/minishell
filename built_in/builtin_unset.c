/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:30:50 by kyoshida          #+#    #+#             */
/*   Updated: 2024/05/14 23:01:48 by yoshidakazu      ###   ########.fr       */
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
    if(ft_strncmp(args[1],"PWD",3) == 0)
    return 0;
    t_var *head;
    head = env_map;
	while (args[i] != NULL)
	{
        env_map = head;
		while (env_map != NULL)
		{
			if (!is_identifier(args[i]))
			{
				g_status = GENERAL_ERRORS;
				command_error(args[i], "unset");
				return (0);
			}
			if (ft_strncmp(args[i], env_map->name, ft_strlen(args[i])) == 0)
            {
				unset_env(args[i], env_map);
                break;                
            }
			env_map = env_map->next;
		}
		i++;
	}
	return (0);
}
