/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:02:57 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/23 19:42:18 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(char *env_name, t_var *map)
{
	size_t	name_len;

	name_len = ft_strlen(env_name);
	while (map != NULL)
	{
		if (ft_strncmp(map->name, env_name, name_len) == 0)
			return (map->value);
		map = map->next;
	}
	return (NULL);
}

static char	*join2value(char *ans, t_var *map)
{
	char	*tmp;

	tmp = ft_strjoin(ans, map->value);
	free(ans);
	ans = tmp;
	free(tmp);
	tmp = ft_strjoin(ans, "\n");
	free(ans);
	ans = tmp;
	free(tmp);
	return (ans);
}

char	*get_env_list(t_var *map)
{
	char	*ans;
	char	*tmp;

	ans = NULL;
	tmp = NULL;
	while (map != NULL)
	{
		if (ans)
		{
			ans = ft_strjoin(ans, map->name);
			tmp = ft_strjoin(ans, "=");
			ans = tmp;
			free(tmp);
		}
		else
			ans = ft_strjoin(map->name, "=");
		ans = join2value(ans, map);
		map = map->next;
	}
	return (ans);
}
