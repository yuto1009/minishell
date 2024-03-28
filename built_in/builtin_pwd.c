/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:29:17 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/28 16:30:26 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int	builtin_pwd(t_var *env_map)
{
	t_var	*tmp;

	tmp = env_map;
	while (tmp)
	{
		if (strncmp(tmp->name, "PWD", 3) == 0)
		{
			printf("%s\n", tmp->value);
			return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}
