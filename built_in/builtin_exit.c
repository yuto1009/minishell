/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 12:08:13 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/11 09:40:33 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}

static bool	check_long_overflow(char *str, int sign)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i == 18)
		{
			if (sign == 1 && str[i] > '7')
				return (false);
			else if (sign == -1 && str[i] > '8')
				return (false);
		}
		else if (i > 18)
			return (false);
		i++;
	}
	return (true);
}
bool str_is_digit(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (false);
        i++;
    }
    return (true);
}

static void	atol_exit(char *str)
{
	int	i;
	int	sign;
	int	num;

	num = 0;
	i = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isdigit(str[i]) || !check_long_overflow(str, sign))
	{
		printf("minishell: exit: %s: numeric argument required",str);
		exit(255);
	}
	while (str[i])
		num = num * 10 + (str[i++] - '0');
	if (num > 256)
		num = num % 256;
	if (sign < 0)
		num = -num;
	exit(num);
}

int	builtin_exit(char **args, int status)
{
	int	arg_len;

	arg_len = 0;
	arg_len = count_args(args);
	printf("exit\n");
    if(!str_is_digit(args[1]) )
    {
		printf("minishell: exit: %s: numeric argument required",args[1]);
		exit(255);
	}
    if(arg_len > 2)
		return (minishell_error("exit : too many arguments"));
    if(arg_len == 2)
		atol_exit(args[1]);
	else
		exit(status);
	return (0);
}
