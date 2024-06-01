/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:04:23 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/06/01 18:49:13 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	free_close(char *line, int pfd[2])
{
	free(line);
	close(pfd[1]);
	return (pfd[0]);
}

int	heredoc(char *delimiter)
{
	char	*line;
	int		pfd[2];

	pipe(pfd);
	while (1)
	{
		signal_heredoc();
		line = readline("> ");
		if (line == NULL)
			return (free_close(line, pfd));
		if (my_strcmp(line, delimiter) == 0)
			return (free_close(line, pfd));
		write(pfd[1], line, ft_strlen(line));
		free(line);
		write(pfd[1], "\n", 1);
	}
	close(pfd[1]);
	return (pfd[0]);
}
