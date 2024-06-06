/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_pid.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:02:20 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/06/06 13:32:25 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	wait_pid(pid_t pid)
{
	pid_t	result;
	int		wstatus;

	while (1)
	{
		result = wait(&wstatus);
		if (result == pid)
		{
			if (WIFSIGNALED(wstatus))
				g_status = 128 + WTERMSIG(wstatus);
			else
				g_status = WEXITSTATUS(wstatus);
		}
		else if (result < 0)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				fatal_error("wait");
		}
	}
	return ;
}
