/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 23:11:52 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/15 11:30:49 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int    find_env_operator(char *str)
{
    if (str == NULL)
        return (INVALID_OP);
	while (*str != '\0' && *str != '+' && *str != '=')
		str++;
	if (*str == '\0')
		return (INVALID_OP);
    if (ft_strnstr(str, APPEND_OP_STR, ft_strlen(APPEND_OP_STR)))
        return (APPEND_OP);
    else if (ft_strnstr(str, ASSIGN_OP_STR, ft_strlen(ASSIGN_OP_STR)))
        return (ASSIGN_OP);
    else
        return (INVALID_OP);
}

static char *configure_operator_pos(char *env, const int env_operator)
{
	if (env_operator == ASSIGN_OP)
		return (ft_strchr(env, '='));
	if (env_operator == APPEND_OP)
		return (ft_strchr(env, '+'));
	return (NULL);
}

char	*trim_env_name(char *env, const int env_operator)
{
	char	*operator_pos;
	char	*name;

	if (env == NULL)
		return (NULL);
	operator_pos = configure_operator_pos(env, env_operator);
	if (operator_pos == NULL)
		return (NULL);
	name = (char *)ft_calloc(operator_pos - env + 1, sizeof(char));
	if (name == NULL)
		fatal_error("Malloc Error");
	ft_strlcpy(name, env, operator_pos - env + 1);
	return (name);
}

char	*trim_env_value(char *env)
{
	char	*equal_pos;
	char	*value;

	if (env == NULL)
		return (NULL);
	equal_pos = ft_strchr(env, EQUAL_SIGN);
	if (equal_pos == NULL)
		return (NULL);
	value = (char *)ft_calloc(ft_strlen(env) - (equal_pos - env + 1) + 1,
			sizeof(char));
	if (value == NULL)
		fatal_error("Malloc Error");
	ft_strlcpy(value, equal_pos + 1, ft_strlen(env) - (equal_pos - env + 1)
		+ 1);
	return (value);
}