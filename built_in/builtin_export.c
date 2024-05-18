/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:10:24 by kyoshida          #+#    #+#             */
/*   Updated: 2024/05/18 18:19:05 by yuendo           ###   ########.fr       */
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

void	handle_env_operation(t_var *env_map, int env_operator, char *env_name,
		char *env_value)
{
	if (env_operator == ASSIGN_OP)
	{
		unset_env(env_name, env_map);
		export_env(env_map, env_name, env_value);
	}
	if (env_operator == APPEND_OP)
		append_env(env_map, env_name, env_value);
}

static int	process_env_argument(char *arg, t_var *env_map)
{
	const int	env_operator = find_env_operator(arg);
	char		*env_name;
	char		*env_value;

	if (env_operator == INVALID_OP)
		return (0);
	env_name = trim_env_name(arg, env_operator);
	if (!env_name && is_identifier(arg))
		return (0);
	env_value = trim_env_value(arg);
	if (!is_identifier(env_name) || (!env_name && !is_identifier(arg))
		|| env_operator == INVALID_OP)
	{
		free(env_name);
		free(env_value);
		g_status = GENERAL_ERRORS;
		return (command_error(arg, "export"));
	}
	handle_env_operation(env_map, env_operator, env_name, env_value);
	return (1);
}

int	builtin_export(char **args, t_var *env_map)
{
	int	i;

	i = 1;
	while (args[i] != NULL)
	{
		if (!process_env_argument(args[i], env_map))
		{
			i++;
			continue ;
		}
		i++;
	}
	return (g_status);
}
