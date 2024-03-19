/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 17:56:12 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/19 18:33:05 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int builtin_echo(char  **args) 
{
    bool is_op_n;
    int i;
    i = 1;
    is_op_n = false;
    // printf("this is echo\n");
    while (args[i] != NULL && ft_strncmp(args[i], "-n", 2) == 0)
    {
        is_op_n = true;
        i++;
    }
    while (args[i] != NULL)
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1] != NULL)
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (!is_op_n)
        ft_putstr_fd("\n", 1);
    return 0;
}
