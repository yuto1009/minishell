/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:03:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/28 16:02:55 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

char	*delete_pwd_elm(char *pwd)
{
	int		i;
	int		last_slash_ptr;
	char	*ans;

	i = 0;
	while (pwd[i] != '\0')
	{
		if (pwd[i] == '/')
			last_slash_ptr = i;
		i++;
	}
	ans = (char *)ft_calloc(sizeof(char), last_slash_ptr + 1);
	ft_strlcpy(ans, pwd, last_slash_ptr + 1);
	free(pwd);
	return (ans);
}

char	*append_pwd(char *pwd, char **path)
{
	int		i;
	char	*src;
	char	*ans;

	src = *path;
	i = 0;
	while ((*path)[i] != '\0' && (*path)[i] != '/')
		i++;
	if (pwd[ft_strlen(pwd) - 1] != '/')
		ft_strlcat(pwd, "/", ft_strlen(pwd) + 2);
	ans = (char *)malloc(sizeof(char) * i + 1);
	ft_strlcpy(ans, src, i + 1);
	*path += i;
	return (ft_strjoin(pwd, ans));
}

char	*new_pwd(char *prev_pwd, char *path)
{
	char	*new_pwd;

	new_pwd = (char *)malloc(sizeof(char) * PATH_MAX);
	if (!prev_pwd)
		return (NULL);
	if (*path == '/')
		ft_strlcpy(new_pwd, "/", PATH_MAX);
	else
		ft_strlcpy(new_pwd, prev_pwd, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (ft_strncmp(path, "..", 2) == 0)
		{
			new_pwd = delete_pwd_elm(new_pwd);
			path += 2;
		}
		else if (ft_strncmp(path, ".", 1) == 0)
			path++;
		else
			new_pwd = append_pwd(new_pwd, &path);
	}
	return (new_pwd);
}

int	builtin_cd(char **args, t_var *env_map)
{
	char	*home;
	char	*path;
	char	*prev_pwd;

	prev_pwd = get_env_value("PWD", env_map);
	unset_env("OLDPWD", env_map);
	export_env(env_map, "OLDPWD", prev_pwd);
	if (args[1] == NULL)
	{
		home = get_env_value("HOME", env_map);
		path = (char *)malloc(sizeof(char) * ft_strlen(home) + 1);
		if (!home)
			return (minishell_error("HOME not set"));
		ft_strlcpy(path, home, ft_strlen(home) + 1);
	}
	else
	{
		path = (char *)malloc(sizeof(char) * ft_strlen(args[1]) + 1);
		ft_strlcpy(path, args[1], ft_strlen(args[1]) + 1);
	}
	if (chdir(path) < 0)
		return (cd_error(path));
	unset_env("PWD", env_map);
	export_env(env_map, "PWD", new_pwd(prev_pwd, path));
	return (0);
}
