/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:10:24 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/28 16:42:42 by kyoshida         ###   ########.fr       */
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

bool	is_env_exists(t_var *env_map, char *name)
{
	t_var	*tmp;

	tmp = env_map;
	if (!name)
		return (false);
	while (tmp != NULL)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name)) == 0)
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

int	builtin_export(char **args, t_var *env_map)
{
	int		i;
	char	*env_name;
	char	*env_value;

	i = 1;
	while (args[i] != NULL)
	{
		env_name = trim_env_name(args[i]);
		if (!env_name && is_identifier(args[i]))
			return (0);
		env_value = trim_env_value(args[i]);
		if (!is_identifier(env_name) || (!env_name && !is_identifier(args[i])))
		{
			g_status = GENERAL_ERRORS;
			return (command_error(args[i], "export"));
		}
		if (is_env_exists(env_map, env_name))
		{
			i++;
			continue ;
		}
		export_env(env_map, env_name, env_value);
		i++;
	}
	return (0);
}
