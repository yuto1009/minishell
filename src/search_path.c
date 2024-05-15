/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 16:44:38 by yuendo            #+#    #+#             */
/*   Updated: 2024/05/15 14:09:49 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	*search_path(char *filename)
{
	extern char	**environ;
	char		*paths;
	char		*path;
	char		*executable;

	paths = getenv("PATH");
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
