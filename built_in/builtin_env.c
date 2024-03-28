/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:45:42 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/28 16:03:39 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int	builtin_env(t_var *env_map)
{
	while (env_map != NULL)
	{
		printf("%s=%s\n", env_map->name, env_map->value);
		env_map = env_map->next;
	}
	return (0);
}
