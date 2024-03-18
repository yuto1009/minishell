/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 17:56:12 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/18 21:32:02 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

// echo関数は動作確認むずいから飛ばす
// 実装も難しくないから後回し！
int builtin_echo(t_token *token) 
{
  bool is_op_n;
    is_op_n = false;
    // printf("this is echo\n");
    token = token->next;
    if(!token)
        return 0;
    while (token->str != NULL && ft_strncmp(token->str, OP_N, 2) == 0)
    {
        is_op_n = true;
        token = token->next;
    }
    while (token->str != NULL)
    {
        ft_putstr_fd(token->str, 1);
        if (token->next->str != NULL)
            ft_putstr_fd(" ", 1);
        token = token->next;
    }
    if (!is_op_n)
        ft_putstr_fd("\n", 1);
    return 0;
}
