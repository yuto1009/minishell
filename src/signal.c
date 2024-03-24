/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:13:26 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/23 17:20:03 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
int g_status;
static void	handler_heredoc(int signum)
{
    if(signum)
    ;
    write(1,"\n",1);
	close(0);
    exit(0);
}

static void	handler_heredoc_quit(int signum)
{
    if(signum)
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


static void reset_sig(int sig)
{
    struct sigaction	sa;

    sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	sigaction(sig, &sa, NULL);
}
static void ignore_sig(int sig)
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

static void sigint_action()
{
    printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0); //コンパイルの都合で消す 使用してるから後で治す
	rl_redisplay();
    g_status = GENERAL_ERRORS;
}

static void sigquit_action()
{
    rl_on_new_line();
	rl_redisplay();
}
void setup_signal()
{
    struct sigaction sa1;
    struct sigaction sa2;
    extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = 0;
    sigemptyset(&sa1.sa_mask);
    sa1.sa_handler = sigint_action;
    sigaction(SIGINT , &sa1 , NULL);
    
    sigemptyset(&sa2.sa_mask);
    sa2.sa_handler = sigquit_action;
    sigaction(SIGQUIT , &sa2 , NULL);

}