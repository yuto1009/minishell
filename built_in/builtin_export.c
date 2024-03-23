/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:10:24 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/23 20:34:09 by yutoendo         ###   ########.fr       */
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
bool is_env_exists(t_var *env_map , char *name)
{
    t_var *tmp;
    tmp = env_map;
    if(!name)
        return false;
    while(tmp!=NULL)
    {
        if(ft_strncmp(tmp->name,name,ft_strlen(name)) == 0)
            return true;
        tmp = tmp->next;
    }
    return false;
}
int builtin_export(char **args,t_var *env_map)
{
    int i;
    i = 1;
	while(args[i]!=NULL)
	{
		char *env_name = (args[i]);
        if(!env_name && is_identifier(args[i]))
            return 0;
		char *env_value = trim_env_value(args[i]);
		if (!is_identifier(env_name)|| (!env_name&&!is_identifier(args[i])))
		{
			g_status = GENERAL_ERRORS;
			printf("minishell: export: '%s': not a valid identifier\n", args[i]);
			return 1;
		}
         if(is_env_exists(env_map,env_name))
         {
            i++;
            continue;
         }
		export_env(env_map, env_name, env_value);
      
        i++;
	}
    return 0;
}
