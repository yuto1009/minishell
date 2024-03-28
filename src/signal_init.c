/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:15:11 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/28 17:18:20 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			g_status;

static void	reset_sig(int sig)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	sigaction(sig, &sa, NULL);
}

void	sigint_action(int signum)
{
	if (signum)
		;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = GENERAL_ERRORS;
}

static void	ignore_sig(int sig)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sigaction(sig, &sa, NULL);
}

void	signal_parent_init(void)
{
	ignore_sig(SIGINT);
	ignore_sig(SIGQUIT);
}

void	signal_child_init(void)
{
	reset_sig(SIGINT);
	reset_sig(SIGQUIT);
}
