/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 12:12:37 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/27 19:57:35 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t sig = 0;

void handler(int signum)
{
    sig = signum;
}

void reset_sig(int signum)
{
    struct sigaction sa;
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_DFL;
    if (sigaction(signum, &sa, NULL) < 0)
    {
        fatal_error("sigaction");
    }
}

void ignore_sig(int signum)
{
    struct sigaction sa;
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_IGN;
    if (sigaction(signum, &sa, NULL) < 0)
    {
        fatal_error("sigaction");
    }
}

void setup_sigint(void)
{
    struct sigaction sa;
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) < 0)
    {
        fatal_error("sigaction");
    }
}

#include <signal.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/types.h>

int check_state(void)
{
    if (sig == 0)
        return (0);
    else if (sig == SIGINT)
    {
        sig = 0;
        readline_interrupted = true;
        rl_replace_line("", 0);
        rl_done = 1;
        return (0);
    }
    return (0);
}

void setup_signal(void)
{
    extern int _rl_echo_control_chars;
    
    _rl_echo_control_chars = 0;
    rl_outstream = stderr;
    if (isatty(STDIN_FILENO))
    {
        rl_event_hook = check_state;
    }
    ignore_sig(SIGQUIT);
    setup_sigint();
}

void reset_signal(void)
{
    reset_sig(SIGQUIT);
    reset_sig(SIGINT);
}