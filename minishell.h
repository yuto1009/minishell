/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:58 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/16 21:47:06 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
    #define MINISHELL_H

// READLINE
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// LIMITS
#include <limits.h>
// ACCESS
#include <unistd.h>
// SIGNAL
# include <signal.h>
#include <sys/types.h>
// WAIT 
#include <sys/wait.h>
// LIBFT
#include "libft/libft.h"

// typedef struct s_token
// {
//     char *token;
//     t_token 
// }   t_token;

// typedef struct s_input
// {
//     char *input;
    
// }   t_input;

#endif