/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:34:36 by yutoendo          #+#    #+#             */
/*   Updated: 2024/01/08 18:46:12 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef BUILT_IN_H
# define BUILT_IN_H

#include "../../include/minishell.h"

//env
typedef struct s_var{
  const char *key;
  const char *value;
  struct s_var *next;
} t_var;



void mini_pwd();
int mini_exit(char **args);
int mini_env(char **argv);
#endif