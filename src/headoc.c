/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:04:23 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/24 14:38:30 by yoshidakazu      ###   ########.fr       */
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
	char *line;
	int pfd[2];

	pipe(pfd);
	signal_heredoc();
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (free_close(line, pfd));
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			return (free_close(line, pfd));
		while (*line != '\0') // tempを使って文字列を操作
		{
			write(pfd[1], line, 1);
			line++;
		}
		write(pfd[1], "\n", 1);
	}
	close(pfd[1]);

	return (pfd[0]);
}