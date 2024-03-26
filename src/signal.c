/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:13:26 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/26 18:58:20 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			g_status;
static void	handler_heredoc(int signum)
{
	if (signum)
		;
	write(1, "\n", 1);
	close(0);
	exit(0);
}

static void	handler_heredoc_quit(int signum)
{
	if (signum)
		;
	rl_on_new_line();
	rl_redisplay();
}

void	signal_heredoc(void)
{
	struct sigaction	act1;
	struct sigaction	act2;

	sigemptyset(&act1.sa_mask);
	act1.sa_handler = handler_heredoc;
	sigaction(SIGINT, &act1, NULL);
	sigemptyset(&act2.sa_mask);
	act2.sa_handler = handler_heredoc_quit;
	sigaction(SIGQUIT, &act2, NULL);
}

void	sigquit_action(int signum)
{
    if(signum)
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
