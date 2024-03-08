/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 16:01:12 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/07 14:08:47 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void exec_builtin(char **str)
{
	if (ft_strncmp(str[0], "echo", 5) == 0)
		mini_echo(str);
	else if (ft_strncmp(str[0], "pwd", 4) == 0)
		mini_pwd();
	
	// else if (ft_strncmp(str[0], "export", 7) == 0)
	// 	mini_export(str);
	// else if (ft_strncmp(str[0], "unset", 6) == 0)
	// 	mini_unset(str);
	// else if (ft_strncmp(str[0], "env", 4) == 0)
	// 	mini_env();
}