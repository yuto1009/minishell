/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:34:36 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/18 21:42:58 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef BUILT_IN_H
# define BUILT_IN_H

#include "../../include/minishell.h"

typedef struct s_var t_var;
typedef struct s_token t_token;
#define OP_N "-n"

void builtin_unset(t_token *token,t_var *env_map);
void builtin_env(t_var *env_map);
void builtin_export(t_token *token,t_var *env_map);
int builtin_pwd();
int builtin_echo(t_token *token);
int builtin_exit(t_token *token,int status);
#endif

