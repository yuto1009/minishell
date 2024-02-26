/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:40:04 by kyoshida          #+#    #+#             */
/*   Updated: 2024/02/26 16:46:59 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



void set_pipe(t_node *node,int end_index)
{
    // printf("hello\n");
    if(node->index == end_index )
        return;
    // if(pipe(node->pipe_out)<0)
    pipe(node->pipe_out);
        //error
    node->next->pipe_in[0] = node->pipe_out[0];
    node->next->pipe_in[1] = node->pipe_out[1];
    
}

void dup_child_pipe(t_node *node)
{
    close(node->pipe_out[0]);
    dup2(node->pipe_in[0],STDIN_FILENO);
    if(node->pipe_in[0] != STDIN_FILENO)
        close(node->pipe_in[0]);
    dup2(node->pipe_out[1],STDOUT_FILENO);
    if(node->pipe_out[1] != STDOUT_FILENO)
        close(node->pipe_out[1]);
}

void set_parent_pipe(t_node *node)
{
    if(node->pipe_in[0] != STDIN_FILENO)
        close(node->pipe_in[0]);
    if(node->next)
        close(node->pipe_out[1]);
}