/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:42:36 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/28 16:15:32 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*tokenize_line(char **line)
{
	t_token	*new;

	new = NULL;
	if (is_blank(**line))
	{
		(*line)++;
		return (NULL);
	}
	if (is_operator(*line))
	{
		new = tokenize_operator(line);
	}
	else if (is_redirection_operator(*line))
	{
		new = tokenize_redirection_operator(line);
	}
	else
	{
		new = tokenize_word(line);
	}
	return (new);
}
