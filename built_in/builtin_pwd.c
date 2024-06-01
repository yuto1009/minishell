/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:29:17 by yutoendo          #+#    #+#             */
/*   Updated: 2024/06/01 17:07:09 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int	builtin_pwd(t_var *env)
{
	char	cwd[PATH_MAX];
	char	*pwd;
	char	*ans;

	pwd = "OLDPWD";
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ans = get_env_value(pwd, env);
		printf("%s\n", ans);
		free(ans);
		return (0);
	}
	printf("%s\n", cwd);
	return (0);
}
