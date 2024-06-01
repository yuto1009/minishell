/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:18:41 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/23 22:09:57 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fatal_error(char *message)
{
	ft_putendl_fd(message, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	cmd_error_exit(char *location, char *message, int exit_status)
{
	const char	*location_message;

	if (!location)
		exit(0);
	location_message = ft_strjoin(location, ": ");
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)location_message, STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	free((char *)location_message);
	exit(exit_status);
}

int	cmd_error_return(char *location, char *message, int exit_status)
{
	const char	*location_message;

	location_message = ft_strjoin(location, ": ");
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)location_message, STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	free((char *)location_message);
	return (exit_status);
}
