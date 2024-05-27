/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:03:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/27 19:39:39 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

static char	*get_target_path(char **args, t_var *env_map)
{
	char	*home;
	char	*path;

	if (args[1] == NULL || ft_strncmp(args[1], "~", 2) == 0
		|| ft_strncmp(args[1], "~\\", 2) == 0)
	{
		home = get_env_value("HOME", env_map);
		if (!home)
			return (NULL);
		path = (char *)malloc(sizeof(char) * ft_strlen(home) + 1);
		ft_strlcpy(path, home, ft_strlen(home) + 1);
		free(home);
	}
	else
	{
		path = (char *)malloc(sizeof(char) * ft_strlen(args[1]) + 1);
		ft_strlcpy(path, args[1], ft_strlen(args[1]) + 1);
	}
	return (path);
}

int	builtin_cd(char **args, t_var *env_map)
{
	char	*path;
	char	*oldpwd_value;
	char	*pwd;
	char	*oldpwd;

	pwd = ft_strdup("PWD");
	oldpwd = ft_strdup("OLDPWD");
	oldpwd_value = get_env_value(pwd, env_map);
	unset_env(oldpwd, env_map);
	export_env(env_map, oldpwd, oldpwd_value);
	path = get_target_path(args, env_map);
	if (!path)
		return (minishell_error("HOME not set"));
	if (chdir(path) < 0)
	{
		free(pwd);
		return (cd_error(path));
	}
	unset_env(pwd, env_map);
	if (oldpwd_value != NULL)
		export_env(env_map, pwd, new_pwd(oldpwd_value, path));
	free(path);
	return (0);
}
