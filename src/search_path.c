/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:44:38 by yuendo            #+#    #+#             */
/*   Updated: 2024/05/23 18:06:41 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_paths(t_var *env_map)
{
	while (env_map != NULL)
	{
		if (my_strcmp(env_map->name, "PATH") == 0)
			return (env_map->value);
		env_map = env_map->next;
	}
	return (NULL);
}

static char	*trim_single_path(char *paths)
{
	size_t	i;
	char	*path;

	i = 0;
	while (paths[i] != '\0')
	{
		if (paths[i] == ':')
			break ;
		i++;
	}
	path = (char *)malloc(sizeof(char) * (i) + 2);
	if (path == NULL)
	{
		fatal_error("Malloc Error");
	}
	ft_strlcpy(path, paths, i + 1);
	ft_strlcat(path, "/", i + 2);
	return (path);
}

char	*search_path(char *filename, t_var *env_map)
{
	char		*paths;
	char		*path;
	char		*executable;

	paths = get_paths(env_map);
	while (paths != NULL)
	{
		path = trim_single_path(paths);
		paths = ft_strchr(paths, ':');
		if (paths != NULL)
			paths++;
		executable = ft_strjoin(path, filename);
		if (executable == NULL)
			fatal_error("Malloc Error");
		if (access(executable, F_OK) == 0)
		{
			free(path);
			return (executable);
		}
		free(executable);
		free(path);
	}
	return (NULL);
}
