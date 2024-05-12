/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:03:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/12 19:24:08 by yoshidakazu      ###   ########.fr       */
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
void check_access_path()
{
    char *path;
    path = getcwd(NULL, 0);
    if(path == NULL)
       printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
    free(path);
}

int	builtin_cd(char **args, t_var *env_map)
{
	char	*path;
	char	*prev_pwd;
	char	*current_pwd;
	char	*old_pwd;

	current_pwd = ft_strdup("PWD");
	old_pwd = ft_strdup("OLDPWD");
	prev_pwd = get_env_value(current_pwd, env_map);
	unset_env(old_pwd, env_map);
	export_env(env_map, old_pwd, prev_pwd);
	path = get_target_path(args, env_map);
	if (!path)
		return (minishell_error("HOME not set"));
	if (chdir(path) < 0)
	{
		free(current_pwd);
		return (cd_error(path));
	}
    check_access_path();
	unset_env(current_pwd, env_map);
	export_env(env_map, current_pwd, new_pwd(prev_pwd, path));
	free(path);
	return (0);
}
