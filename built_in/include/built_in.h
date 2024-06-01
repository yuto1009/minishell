/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:34:36 by yutoendo          #+#    #+#             */
/*   Updated: 2024/06/01 16:02:06 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "../../include/minishell.h"
# include <stdbool.h>
typedef struct s_var	t_var;
typedef struct s_token	t_token;
# define OP_N "-n"

int						builtin_unset(char **args, t_var *env_map);
int						builtin_env(t_var *env_map);
int						builtin_export(char **args, t_var *env_map);
int						builtin_pwd(t_var *env);
int						builtin_echo(char **args);
int						builtin_exit(char **args, int status);
char					*delete_pwd_elm(char *pwd);
char					*append_pwd(char *pwd, char **path);
char					*new_pwd(char *prev_pwd, char *path);
int						builtin_cd(char **args, t_var *env_map);
int						cd_error(char *cmd);
int						command_error(char *args, char *command);
bool					is_env_exists(t_var *env_map, char *name);
#endif
