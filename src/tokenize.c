/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 18:57:26 by yutoendo          #+#    #+#             */
/*   Updated: 2024/04/25 21:51:28 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	add_token_to_list(t_token **head, t_token **current,
		t_token *newtoken)
{
	if (*head == NULL)
	{
		*head = newtoken;
		(*head)->prev = new_token(NULL, TK_EOF);
		*current = newtoken;
	}
	else
	{
		(*current)->next = newtoken;
		newtoken->prev = *current;
		*current = newtoken;
	}
}

static t_token	*build_token_list(char *line)
{
	t_token	*head;
	t_token	*current;
	t_token	*new;

	head = NULL;
	current = NULL;
	new = NULL;
	while (*line != '\0')
	{
		new = tokenize_line(&line);
		if (new != NULL)
			add_token_to_list(&head, &current, new);
	}
	if (head == NULL)
		head = new_token(NULL, TK_EOF);
	else if (current != NULL)
	{
		current->next = new_token(NULL, TK_EOF);
		current->next->prev = current;
	}
	return (head);
}

t_token	*tokenize(char *line)
{
	return (build_token_list(line));
}
