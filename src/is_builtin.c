/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 15:25:48 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/02 17:49:57 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_builtin(char **cmd)
{
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		return (true);
	else if (ft_strncmp(cmd[0], "cd", 3) == 0)
		return (true);
	else if (ft_strncmp(cmd[0], "pwd", 4) == 0)
		return (true);
	else if (ft_strncmp(cmd[0], "export", 7) == 0)
		return (true);
	else if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (true);
	else if (ft_strncmp(cmd[0], "env", 4) == 0)
		return (true);
	return (0);
}