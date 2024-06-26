/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 15:54:37 by yutoendo          #+#    #+#             */
/*   Updated: 2024/06/01 18:57:25 by kyoshida         ###   ########.fr       */
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
	token = remove_void_tokens(token);
	if (token->kind == TK_EOF)
	{
		if (token->prev)
			free(token->prev);
		return (free(token->str), free(token), NULL);
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
		token2argv = search_redir(node, count_token_len(node->token), 1);
		if (!token2argv)
			return ;
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
	if (node)
		free_node(node);
	return ;
}
