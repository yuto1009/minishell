/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 19:04:31 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/14 23:17:14 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_var	*init_env_map(void)
{
	extern char	**environ;
	char		**env_p;
	char		*name;
	char		*value;
	t_var		*env_map;

	env_p = environ;
	env_map = NULL;
	while (*env_p != NULL)
	{
		name = trim_env_name(*env_p, ASSIGN_OP);
		value = trim_env_value(*env_p);
		env_map = export_env(env_map, name, value);
		env_p++;
	}
	return (env_map);
}
