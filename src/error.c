/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:22:40 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/26 17:32:39 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_output_destination(FILE *dst)
{
	rl_outstream = dst;
}

int	minishell_error(char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
    return (1);
	// write(1,"\n",1);
	// exit(MINISHELL_ERROR);
}
int	cd_error(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
    return (1);
}

int	syntax_error_exit(char *token_str)
{
	const char	*join_str = ft_strjoin(token_str, "'");

	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
	ft_putstr_fd((char *)join_str, STDERR_FILENO);
	write(1, "\n", 1);
	free((char *)join_str);
	return (258);
}

int	unsupported_token_msg(char *str)
{
	const char *join_str = ft_strjoin("sorry this command is unsupported : ",
			str);
	ft_putstr_fd((char *)join_str, STDERR_FILENO);
	write(1, "\n", 1);
	return (-1);
}