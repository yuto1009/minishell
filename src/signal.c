/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:13:26 by kyoshida          #+#    #+#             */
/*   Updated: 2024/06/06 13:32:05 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handler(int signum)
{
	if (signum == SIGQUIT)
		printf("Quit: 3\n");
	else if (signum == SIGINT)
		printf("\n");
	rl_redisplay();
}

void	sigquit_action(int signum)
{
	if (signum)
		;
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signal(void)
{
	struct sigaction	sa1;
	struct sigaction	sa2;
	extern int			_rl_echo_control_chars;

	_rl_echo_control_chars = 0;
	sigemptyset(&sa1.sa_mask);
	sa1.sa_handler = sigint_action;
	sigaction(SIGINT, &sa1, NULL);
	sigemptyset(&sa2.sa_mask);
	sa2.sa_handler = sigquit_action;
	sigaction(SIGQUIT, &sa2, NULL);
}
