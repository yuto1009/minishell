/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:34:36 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/17 19:34:29 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef BUILT_IN_H
# define BUILT_IN_H

#include "../../include/minishell.h"

typedef struct s_var t_var;
typedef struct s_token t_token;
#define OP_N "-n"

void builtin_unset(t_token *token,t_var *env_map);
void mini_pwd();
int mini_exit(char **args);
void mini_env(t_var *env_map);
void builtin_export(t_token *token,t_var *env_map);
void mini_pwd();
void mini_echo(t_token **token);
int mini_exit(char **args);
#endif

