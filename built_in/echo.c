/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 17:56:12 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/18 10:59:18 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

// echo関数は動作確認むずいから飛ばす
// 実装も難しくないから後回し！
void buildin_echo(char **str) 
{
  bool is_op_n;
    int i;
    i = 1;
    is_op_n = false;
    // printf("this is echo\n");
    while (str[i] != NULL && ft_strncmp(str[i], "-n", 2) == 0)
    {
        is_op_n = true;
        i++;
    }
    while (str[i] != NULL)
    {
        ft_putstr_fd(str[i], 1);
        if (str[i + 1] != NULL)
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (!is_op_n)
        ft_putstr_fd("\n", 1);
    exit(0);
}
