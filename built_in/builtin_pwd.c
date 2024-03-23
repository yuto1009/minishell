/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:29:17 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/23 13:15:33 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"


int builtin_pwd(t_var *env_map)
{
    t_var *tmp;
    tmp = env_map;
    while(tmp)
    {
        if(ft_strncmp(tmp->name,"PWD",3) == 0)
            printf("%s\n",tmp->value);
        tmp = tmp->next;
    }
    return 0;
}

// int builtin_pwd(t_var *env_map)
// {
//     char *buff = ft_calloc(PATH_MAX, 1);
//     if (buff == NULL)
//         fatal_error("Malloc Error");
//     if (getcwd(buff, PATH_MAX) == NULL) // ユーザーの現在位置を絶対パスで取得する
//     {
//         free(buff);
//         fatal_error("The provided pathname exceeds the maximum length limit of 1024 characters. Please use a shorter pathname.");
//     }
//     printf("%s\n", buff);
//     free(buff);
//     return 0;
// }

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