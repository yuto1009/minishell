/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:45:42 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/27 20:25:10 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int	builtin_env(t_var *env_map)
{
	while (env_map != NULL)
	{
		if (env_map->value == NULL)
			printf("%s=", env_map->name);
		else
			printf("%s=%s\n", env_map->name, env_map->value);
		env_map = env_map->next;
	}
	return (0);
}
