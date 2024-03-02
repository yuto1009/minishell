/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 16:01:12 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/02 17:30:55 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void exec_builtin(char **str)
{
	if (ft_strncmp(str[0], "echo", 5) == 0)
		mini_echo(str);
	// else if (ft_strncmp(str[0], "cd", 3) == 0)
	// 	mini_cd(str);
	// else if (ft_strncmp(str[0], "pwd", 4) == 0)
	// 	mini_pwd();
	// else if (ft_strncmp(str[0], "export", 7) == 0)
	// 	mini_export(str);
	// else if (ft_strncmp(str[0], "unset", 6) == 0)
	// 	mini_unset(str);
	// else if (ft_strncmp(str[0], "env", 4) == 0)
	// 	mini_env();
}