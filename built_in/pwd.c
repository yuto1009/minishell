/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:29:17 by yutoendo          #+#    #+#             */
/*   Updated: 2024/01/03 15:09:57 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void mini_pwd()
{
    char *buff = ft_calloc(PATH_MAX, 1);
    if (buff == NULL)
        fatal_error("Malloc Error");
    if (getcwd(buff, PATH_MAX) == NULL)
    {
        free(buff);
        fatal_error("The provided pathname exceeds the maximum length limit of 1024 characters. Please use a shorter pathname.");
    }
    printf("%s\n", buff);
    free(buff);
}

// minishellに接続しないで動作確認するためのコード 
// #include <unistd.h>
// #include <stdio.h>
// #include <limits.h>
// #include <stdlib.h>

// void mini_pwd()
// {
//     char *buff = (char *)calloc(PATH_MAX, 1);
//     if (buff == NULL)
//     {   
//         printf("Malloc Error");
//         return;
//     }
//     getcwd(buff, PATH_MAX);
//     if (buff == NULL)
//         printf("The provided pathname exceeds the maximum length limit of 1024 characters. Please use a shorter pathname.");
//     printf("%s\n", buff);
//     free(buff);
// }

// int main(void)
// {
//     mini_pwd();
//     return (0);
// }