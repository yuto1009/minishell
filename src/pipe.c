/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 20:58:31 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/26 21:15:39 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void copy_pipe(int dst[2], int src[2]);

void prepare_pipe(t_node *node)
{
    if (node->next == NULL)
        return ;
    if (pipe(node->outpipe) < 0)
    {
        fatal_error("pipe");
    }
    copy_pipe(node->next->inpipe, node->outpipe);
}

void prepare_pipe_child(t_node *node)
{
    close(node->outpipe[0]);
    dup2(node->inpipe[0], STDIN_FILENO);
    if (node->inpipe[0] != STDIN_FILENO)
    {
        close(node->inpipe[0]);
    }
    dup2(node->outpipe[1], STDOUT_FILENO);
    if (node->outpipe[1] != STDOUT_FILENO)
    {
        close(node->outpipe[1]);
    }
}

void prepare_pipe_parent(t_node *node)
{
    if (node->inpipe[0] != STDIN_FILENO)
    {
        close(node->inpipe[0]);
    }
    if (node->next != NULL)
    {
        close(node->outpipe[1]);
    }
}

static void copy_pipe(int dst[2], int src[2])
{
    dst[0] = src[0];
    dst[1] = src[1];
}