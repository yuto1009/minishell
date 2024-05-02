/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:54:37 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/02 23:06:23 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_token_error(t_token *token)
{
	t_token	*tmp;

	free(token->prev);
	while (token != NULL)
	{
		free(token->str);
		tmp = token;
		token = token->next;
		free(tmp);
	}
}

static t_token	*interpret_line2token(char *line, t_var *env_map,
		int prev_status)
{
	t_token	*token;
	int		status;

	token = tokenize(line);
	if (expand(token, env_map, prev_status) < 0)
	{
		free_token_error(token);
		return (NULL);
	}
	status = tokenize_error(token);
	if (status == SYNTAX_ERROR || status == CMD_NOT_FOUND_ERROR)
	{
		free_token_error(token);
		g_status = status;
		return (NULL);
	}
	return (token);
}

static void	exec_command(t_node *node, t_var *env_map, int prev_status)
{
	char	**token2argv;
	pid_t	pid;

	if (!node)
		return ;
	if (node->next == NULL && is_builtin(node->token->str))
	{
		token2argv = search_redir(node, count_token_len(node->token));
		if (!token2argv)
			exit(1);
		dup_fd(node);
		g_status = exec_builtin(token2argv, env_map, prev_status);
		reset_fd(node);
		free(token2argv);
	}
	else
	{
		pid = exec(node, env_map, prev_status);
		wait_pid(pid);
	}
}

void	free_node(t_node *node)
{
	t_token	*current_token;
	t_node	*tmpnode;
	t_token	*tmp_token;

	if (!node)
		return ;
	free(node->token->prev);
	while (node != NULL)
	{
		current_token = node->token;
		while (current_token != NULL)
		{
			if (current_token->kind == TK_EOF)
			{
				free(current_token);
				break ;
			}
			free(current_token->str);
			tmp_token = current_token;
			current_token = current_token->next;
			free(tmp_token);
		}
		tmpnode = node;
		node = node->next;
		free(tmpnode);
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
	if (!token)
		return ;
	node = parser(token);
	exec_command(node, env_map, prev_status);
	free_node(node);
	return ;
}
