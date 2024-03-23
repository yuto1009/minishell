/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:54:37 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/23 16:06:21 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token	*interpret_line2token(char *line, t_var *env_map,
		int prev_status)
{
	t_token	*token;
	int		status;

	token = tokenize(line);
	if (expand(token, env_map, prev_status) < 0)
		return (NULL);
	status = tokenize_error(token);
	if (status == SYNTAX_ERROR || status == CMD_NOT_FOUND_ERROR)
	{
		free(token);
		g_status = status;
		return (NULL);
	}
	return (token);
}

static void	exec_command(t_node *node, t_var *env_map, int prev_status)
{
	char	**token2argv;
	pid_t	pid;

	if (node->next == NULL && is_buildin(node->token->str))
	{
		token2argv = serch_redir(node, count_token_len(node->token));
		if (!token2argv)
			exit(1);
		dup_fd(node);
		g_status = exec_buildin(token2argv, env_map, prev_status);
		reset_fd(node);
	}
	else
	{
		pid = exec(node, env_map, prev_status);
		wait_pid(pid);
	}
}

void	interpret(char *line, t_var *env_map)
{
	struct s_node	*node;
	int				prev_status;
	t_token			*token;

	prev_status = g_status;
	g_status = 0;
	token = interpret_line2token(line, env_map, prev_status);
	node = parser(token);
	exec_command(node, env_map, prev_status);
	return ;
}
