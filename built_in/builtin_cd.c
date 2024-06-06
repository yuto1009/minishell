/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:03:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/06/06 14:05:14 by kyoshida         ###   ########.fr       */
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

int	path_check(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		return (-1);
	}
	return (S_ISDIR(path_stat.st_mode));
}

int	check_is_directory(const char *path)
{
	int	result;

	result = path_check(path);
	if (result == 0)
	{
		printf("minishell: cd: %s: Not a directory\n", path);
		return (-1);
	}
	return (0);
}

int	builtin_cd(char **args, t_var *env_map)
{
	char	*path;
	char	*pwd;
	char	*oldpwd;
	char	*oldpwd_value;

	pwd = ft_strdup("PWD");
	oldpwd = ft_strdup("OLDPWD");
	unset_env(oldpwd, env_map);
	oldpwd_value = get_env_value(pwd, env_map);
	export_env(env_map, oldpwd, oldpwd_value);
	path = get_target_path(args, env_map);
	if (!path)
		return (minishell_error("HOME not set"));
	if (check_is_directory(path) < 0)
		return (free(pwd), free(path), 1);
	if (chdir(path) < 0)
	{
		free(pwd);
		return (cd_error(path));
	}
	check_access_path();
	unset_env(pwd, env_map);
	export_env(env_map, pwd, getcwd(NULL, 0));
	free(path);
	return (0);
}
