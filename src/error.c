/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 16:55:50 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/17 23:01:24 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void fatal_error(const char *msg)
{
    dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void assert_error(const char *msg)
{
    dprintf(STDERR_FILENO, "Assert Error: %s\n", msg);
    exit(255);   
}

void err_exit(const char *location, const char *msg, int status)
{
    dprintf(STDERR_FILENO, "minishell: %s %s\n", location, msg);
    exit(status);   
}

void todo(const char *msg)
{
    dprintf(STDERR_FILENO, "TODO: %s\n", msg);
    exit(255);
}