/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:45:42 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/06/01 16:47:23 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int	builtin_env(t_var *env_map)
{
	while (env_map != NULL)
	{
		if (env_map->value == NULL)
			printf("%s=\n", env_map->name);
		else
			printf("%s=%s\n", env_map->name, env_map->value);
		env_map = env_map->next;
	}
	return (0);
}
