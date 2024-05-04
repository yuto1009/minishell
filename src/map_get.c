/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:02:57 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/03 10:32:13 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	my_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char	*get_env_value(char *env_name, t_var *map)
{
	while (map != NULL)
	{
		if (my_strcmp(map->name, env_name) == 0)
			return (ft_strdup(map->value));
		map = map->next;
	}
	return (NULL);
}
