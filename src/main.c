/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/04/14 11:01:54 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

int	count_token_len(t_token *token)
{
	int		i;
	t_token	*tmp;

	tmp = token;
	i = 0;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	free(tmp);
	return (i - 1);
}

static bool	is_only_blank_character(char *line)
{
	bool	ans;

	ans = false;
	if (strcmp(line, "") == 0)
		return (true);
	while (is_blank(*line) && *line != '\0')
	{
		ans = true;
		line++;
	}
	if (*line != '\0')
		ans = false;
	return (ans);
}

static void	roop_readline(void)
{
	t_var	*env_map;
	char	*line;

	env_map = init_env_map();
	set_output_destination(stderr);
	g_status = 0;
	while (1)
	{
		setup_signal();
		line = readline("minishell$ ");
		if (line == NULL)
		{
			break ;
		}
		if (is_only_blank_character(line))
			continue ;
		if (*line)
			add_history(line);
		interpret(line, env_map);
		free(line);
	}
	return ;
}

__attribute__((destructor))
static void destructor() {
    system("leaks -q minishell");
}

int	main(void)
{
	set_output_destination(stderr);
	roop_readline();
	printf("exit\n");
	return (g_status);
}
