/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:29:17 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/17 23:45:16 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

void buildin_pwd()
{
    char *buff = ft_calloc(PATH_MAX, 1);
    if (buff == NULL)
        fatal_error("Malloc Error");
    if (getcwd(buff, PATH_MAX) == NULL) // ユーザーの現在位置を絶対パスで取得する
    {
        free(buff);
        fatal_error("The provided pathname exceeds the maximum length limit of 1024 characters. Please use a shorter pathname.");
    }
    printf("%s\n", buff);
    free(buff);
    exit(0);
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