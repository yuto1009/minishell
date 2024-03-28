/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:44:21 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/28 16:07:28 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	process_quoted_word(char **line, char *word, size_t *i, size_t *j)
{
	char	current_quote;

	current_quote = (*line)[(*i)++];
	word[(*j)++] = current_quote;
	while ((*line)[*i] != '\0')
	{
		word[(*j)++] = (*line)[*i];
		if (current_quote == (*line)[*i])
		{
			(*i)++;
			break ;
		}
		(*i)++;
	}
}

static t_token	*create_word_token(char *word)
{
	return (new_token(word, TK_WORD));
}

t_token	*tokenize_word(char **line)
{
	char	*word;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	word = (char *)ft_calloc(ft_strlen(*line) + 1, sizeof(char));
	if (word == NULL)
		fatal_error("malloc error");
	while ((*line)[i] != '\0' && !is_metacharacter((*line)[i]))
	{
		if ((*line)[i] == SINGLE_QUOTE || (*line)[i] == DOUBLE_QUOTE)
		{
			process_quoted_word(line, word, &i, &j);
		}
		else
		{
			word[j++] = (*line)[i++];
		}
	}
	word[j] = '\0';
	*line += i;
	return (create_word_token(word));
}
