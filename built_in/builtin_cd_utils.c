/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:17:15 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/02 22:19:29 by yutoendo         ###   ########.fr       */
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
	char	*joinans;

	src = *path;
	i = 0;
	while ((*path)[i] != '\0' && (*path)[i] != '/')
		i++;
	if (pwd[ft_strlen(pwd) - 1] != '/')
		ft_strlcat(pwd, "/", ft_strlen(pwd) + 2);
	ans = (char *)malloc(sizeof(char) * i + 1);
	ft_strlcpy(ans, src, i + 1);
	*path += i;
	joinans = ft_strjoin(pwd, ans);
	free(pwd);
	free(ans);
	return (joinans);
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
