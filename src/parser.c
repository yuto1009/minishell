/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/04/25 19:22:49 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*create_command_node(t_token *start_token)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (!new_node)
	{
		fatal_error("Malloc");
	}
	new_node->token = start_token;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->currentin_fd = 0;
	new_node->currentout_fd = 1;
	new_node->redirout_fd = 1;
	new_node->redirin_fd = 0;
	new_node->pipe_in[0] = STDIN_FILENO;
	new_node->pipe_out[1] = STDOUT_FILENO;
	new_node->pipe_out[0] = -1;
	new_node->pipe_in[1] = -1;
	return (new_node);
}

static t_token	*cut_command_tokens(t_token **current,
									t_token **next_command_start)
{
	t_token	*start;
	t_token	*end;

	if (!*current)
		return (NULL);
	start = *current;
	end = start;
	while (end->next != NULL && end->next->kind != TK_EOF
		&& end->kind != TK_OPERATOR)
	{
		end = end->next;
	}
	if (end != NULL && end->kind == TK_OPERATOR)
	{
		*next_command_start = end->next;
		end->kind = TK_EOF;
		free(end->str);
		end = NULL;
	}
	else
		*next_command_start = NULL;
	return (start);
}

t_node	*parser(t_token *tokens)
{
	t_node	*head;
	t_node	*tail;
	t_token	*current_token;
	t_token	*next_command_start;
	t_node	*new_command;

	head = NULL;
	tail = NULL;
	current_token = tokens;
	next_command_start = NULL;
	while (current_token != NULL)
	{
		new_command = create_command_node(cut_command_tokens(&current_token,
					&next_command_start));
		if (head == NULL)
			head = new_command;
		else
		{
			tail->next = new_command;
			new_command->prev = tail;
		}
		tail = new_command;
		current_token = next_command_start;
	}
	return (head);
}
