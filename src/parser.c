/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/01/14 17:40:30 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

typedef struct s_token t_token;
typedef struct s_node t_node;

struct s_node {
  t_node right;
    
};

void parser(t_token *token)
{
  
	while (token->kind != TK_EOF)
    {
        if (token->kind == TK_OPERATOR && strncmp(token->str,";",1) == 0)
        {
          token = token->prev ;
          
        }
    }
}