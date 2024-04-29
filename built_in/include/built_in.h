/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:34:36 by yutoendo          #+#    #+#             */
/*   Updated: 2024/04/29 22:41:41 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "../../include/minishell.h"

typedef struct s_var	t_var;
typedef struct s_token	t_token;
# define OP_N "-n"

int						builtin_unset(char **args, t_var *env_map);
int						builtin_env(t_var *env_map);
int						builtin_export(char **args, t_var *env_map);
int						builtin_pwd(t_var *env_map);
int						builtin_echo(char **args);
int						builtin_exit(char **args, int status);
int						builtin_cd(char **args, t_var *env_map);
int						cd_error(char *cmd);
int						command_error(char *args, char *command);
#endif
