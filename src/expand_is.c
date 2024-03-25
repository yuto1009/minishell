/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_is.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 12:46:18 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/25 12:47:01 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


bool	is_dollar_sign(char c)
{
	return (c == '$');
}

bool	is_single_dollar_sign(char *str)
{
	char	*p;

	p = str;
	if (*p == DOLLAR_SIGN)
	{
		p++;
		if (*p == '\0' || *p == ' ' || *p == '\"' || *p == '\"')
			return (true);
	}
	return (false);
}

bool	is_env_variable(char *str)
{
	char	*p;

	p = str;
	if (*p == DOLLAR_SIGN)
	{
		p++;
		if (*p >= 'A' && *p <= 'Z')
			return (true);
	}
	return (false);
}
bool	is_alpha(char chr)
{
	return (('a' <= chr && chr <= 'z') || ('A' <= chr && chr <= 'Z'));
}


bool	is_exit_status(char *str)
{
	char	*p;

	p = str;
	if (*p == DOLLAR_SIGN)
	{
		p++;
		if (*p == QUESTION)
			return (true);
	}
	return (false);
}