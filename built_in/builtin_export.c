/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:10:24 by kyoshida          #+#    #+#             */
/*   Updated: 2024/05/18 15:12:11 by kyoshida         ###   ########.fr       */
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
		// if(is_blank(*name))
		//     return (false);
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

// static int builtin_export_case_assign();
// static int builtin_export_case_append();

void handle_env_operation(t_var *env_map, int env_operator, char *env_name, char *env_value)
{
	if (env_operator == ASSIGN_OP)
	{
		unset_env(env_name, env_map);
		export_env(env_map, env_name, env_value);
	}
	if (env_operator == APPEND_OP)
		append_env(env_map, env_name, env_value);
}


int	builtin_export(char **args, t_var *env_map)
{
	int			i;
	char		*env_name;
	char		*env_value;
	int	env_operator;

	i = 1;
	while (args[i] != NULL)
	{
		env_operator = find_env_operator(args[i]);
		if (env_operator == INVALID_OP && find_env_operator(args[i + 1]))
		{
			g_status = GENERAL_ERRORS;
			command_error(args[i + 1], "export");
			i += 2;
			continue ;
		}
		env_name = trim_env_name(args[i], env_operator);
		if (!env_name && is_identifier(args[i]))
			return (0);
		env_value = trim_env_value(args[i]);
		if (!is_identifier(env_name) || (!env_name && !is_identifier(args[i]))
			|| env_operator == INVALID_OP)
		{
			g_status = GENERAL_ERRORS;
			return (command_error(args[i], "export"));
		}
		handle_env_operation(env_map, env_operator, env_name, env_value);
		i++;
	}
	return (g_status);
}
