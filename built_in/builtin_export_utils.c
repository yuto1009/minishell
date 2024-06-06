/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:00:23 by yutoendo          #+#    #+#             */
/*   Updated: 2024/06/04 14:10:45 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

void	export_no_args(t_var *env_map)
{
	while (env_map != NULL)
	{
		printf("declare -x ");
		if (env_map->value == NULL)
			printf("%s=\n", env_map->name);
		else
			printf("%s=\"%s\"\n", env_map->name, env_map->value);
		env_map = env_map->next;
	}
}
