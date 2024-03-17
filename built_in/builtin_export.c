/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:10:24 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/17 20:51:23 by kyoshida         ###   ########.fr       */
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

void builtin_export(t_token *token,t_var *env_map)
{
	token = token->next;
	while(token->kind != TK_EOF)
	{
		char *env_name = trim_env_name(token->str);
		char *env_value = trim_env_value(token->str);
		if (is_identifier(env_name) != true)
		{
			g_status = GENERAL_ERRORS;
			printf("minishell: export: '%s': not a valid identifier\n", env_name);
			return ;
		}
		export_env(env_map, env_name, env_value);
	 
		
		// free(env_name);
		// free(env_value);
		token = token->next;
	}
}