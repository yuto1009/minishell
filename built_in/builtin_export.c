/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:10:24 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/19 18:31:56 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// 環境変数名は最初は英文字、そのあとは英数字かアンダースコアである必要がある
static bool is_identifier(char *name)
{
	if (name == NULL || *name == '\0')
		return (false);
	if (!ft_isalpha(*name) && *name != '_')
		return (false);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

void builtin_export(char **args,t_var *env_map)
{
    int i;
    i = 1;
	while(args[i]!=NULL)
	{
		char *env_name = trim_env_name(args[i]);
		char *env_value = trim_env_value(args[i]);
		if (is_identifier(env_name) != true)
		{
			g_status = GENERAL_ERRORS;
			printf("minishell: export: '%s': not a valid identifier\n", env_name);
			return ;
		}
		export_env(env_map, env_name, env_value);
        i++;
	}
}