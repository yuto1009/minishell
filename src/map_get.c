/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:02:57 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/23 21:20:05 by yutoendo         ###   ########.fr       */
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
