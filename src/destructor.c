/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 23:00:42 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/22 16:56:31 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_token(t_token *token)
{
    if (token == NULL)
        return ;
    if (token->word)
        free(token->word);
    free_token(token->next);
    free(token);
}

void free_argv(char **argv)
{
    int i;
    
    if (argv == NULL)
        return ;
    i = 0;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}

void free_node(t_node *node)
{
    if (node == NULL)
        return ;
    free_token(node->args);
    free_node(node->next);
    free(node);
}