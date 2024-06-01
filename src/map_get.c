/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:02:57 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/06/01 16:42:22 by yuendo           ###   ########.fr       */
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
		if (my_strcmp(map->name, env_name) == 0 && map->value != NULL)
			return (ft_strdup(map->value));
		map = map->next;
	}
	return (NULL);
}
