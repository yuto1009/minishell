/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_is.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:47:24 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/02 22:27:13 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_operator(char *line)
{
	const char	*operators[] = {"||", "&&", "&", ";", ";;", "(", ")", "|", "\n"};
	size_t		i;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (ft_strncmp(line, operators[i], ft_strlen(operators[i])) == 0)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

bool	is_redirection_operator(char *line)
{
	const char	*operators[] = {">>", "<<", "<>", "<", ">"};
	size_t		i;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (ft_strncmp(line, operators[i], ft_strlen(operators[i])) == 0)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_metacharacter(char c)
{
	const char	*metacharacters;

	metacharacters = "| << >> < > \t";
	if (ft_strchr(metacharacters, c) != NULL)
	{
		return (true);
	}
	return (false);
}
