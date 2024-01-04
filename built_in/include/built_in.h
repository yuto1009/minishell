/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:34:36 by yutoendo          #+#    #+#             */
/*   Updated: 2024/01/04 20:13:47 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#define OP_N "-n"
typedef struct s_token t_token;
void mini_pwd();
void mini_echo(t_token **token);
int mini_exit(char **args);

