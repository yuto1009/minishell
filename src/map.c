/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:23:23 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/12 23:04:52 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_var	*create_map(char *name, char *value)
{
	t_var	*map;

	map = (t_var *)ft_calloc(1, sizeof(t_var));
	if (map == NULL)
		fatal_error("malloc error");
	map->name = name;
	map->value = value;
	map->prev = NULL;
	map->next = NULL;
	return (map);
}

t_var	*export_env(t_var *map, char *env_name, char *env_value)
{
	if (map == NULL)
		map = create_map(env_name, env_value);
	else
	{
		while (map->next != NULL)
			map = map->next;
		map->next = create_map(env_name, env_value);
		map->next->prev = map;
		while (map->prev != NULL)
			map = map->prev;
	}
	return (map);
}

t_var	*append_env(t_var *map, char *env_name, char *additional_value)
{
	char *tmp_env_value;
	t_var *head;
	
	if (map == NULL)
		return (create_map(env_name, additional_value));
	else
	{
		head = map;
		while (map != NULL)
		{
			if (ft_strncmp(map->name, env_name, ft_strlen(env_name)) == 0 && ft_strlen(map->name) == ft_strlen(env_name))
			{
				tmp_env_value = map->value;
				map->value = ft_strjoin(map->value, additional_value);
				free(tmp_env_value);
				return (head);
			}
			if (map->next == NULL)
			{
				map->next = create_map(env_name, additional_value);
				map->next->prev = map;
				return (head);
			}
			map = map->next;
		}
	}
	return (map);
}

void	unset_env(char *env_name, t_var *map)
{
	t_var	*current;
	t_var	*next;

	current = map;
	while (current != NULL)
	{
		next = current->next;
		if (ft_strncmp(current->name, env_name, ft_strlen(current->name)) == 0)
		{
			if (current->prev != NULL)
				current->prev->next = current->next;
			if (current->next != NULL)
				current->next->prev = current->prev;
			free(current->name);
			free(current->value);
			free(current);
			break ;
		}
		current = next;
	}
}

