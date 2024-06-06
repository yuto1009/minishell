/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 17:11:58 by yuendo            #+#    #+#             */
/*   Updated: 2024/06/06 13:38:32 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	exec_in_order(char **token2argv, t_var *env_map, int prev_status)
{
	if (is_builtin(token2argv[0]))
		exit(exec_builtin(token2argv, env_map, prev_status));
	else
		execute_pipe(token2argv, env_map);
}

int	exec(t_node *node, t_var *env_map, int prev_status)
{
	int		pid;
	char	**token2argv;

	while (node != NULL)
	{
		signal_parent_init();
		set_pipe(node);
		pid = fork();
		if (pid < 0)
			return (cmd_error_return("fork", "fork error", 1));
		else if (pid == 0)
		{
			dup_child_pipe(node);
			token2argv = search_redir(node, count_token_len(node->token), 0);
			if (!token2argv)
				exit(1);
			dup_fd(node);
			exec_in_order(token2argv, env_map, prev_status);
		}
		set_parent_pipe(node);
		node = node->next;
	}
	return (pid);
}
