/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:50:45 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/02 22:24:47 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*tokenize_operator(char **line)
{
	const char	*operators[] = {"||", "&&", ";;", "&", ";", "(", ")", "|", "\n"};
	char		*operator;
	size_t		i;

	operator = NULL;
	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (ft_strncmp(*line, operators[i], ft_strlen(operators[i])) == 0)
		{
			break ;
		}
		i++;
	}
	operator = ft_substr(*line, 0, ft_strlen(operators[i]));
	if (operator == NULL)
		fatal_error("malloc error");
	*line += ft_strlen(operator);
	return (new_token(operator, TK_OPERATOR));
}

t_token	*tokenize_redirection_operator(char **line)
{
	const char	*operators[] = {">>", "<<", "<>", "<", ">"};
	char		*operator;
	size_t		i;

	operator = NULL;
	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (ft_strncmp(*line, operators[i], ft_strlen(operators[i])) == 0)
		{
			break ;
		}
		i++;
	}
	operator = ft_substr(*line, 0, ft_strlen(operators[i]));
	if (operator == NULL)
		fatal_error("malloc error");
	*line += ft_strlen(operator);
	return (new_token(operator, TK_REDIRECTION));
}
