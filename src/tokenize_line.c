/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:42:36 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/04/29 14:26:46 by yoshidakazu      ###   ########.fr       */
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
