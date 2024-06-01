/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:29:17 by yutoendo          #+#    #+#             */
/*   Updated: 2024/06/01 16:35:48 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int	builtin_pwd(t_var *env)
{
	char cwd[PATH_MAX];
	char *pwd = "OLDPWD";
	char *ans;
	
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ans = get_env_value(pwd ,env);
		printf("%s\n",ans);    
		// printf("hogeghoge\n");
		return 0;
	}
		// fatal_error("cwd error");
	printf("%s\n", cwd);
	return (0);
}
