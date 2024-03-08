/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:03:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/07 16:37:10 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

void mini_cd(char **str)   // t_tokenの形で入力がある想定
{
    char *path = str[1];   // 1つ目の引数はcdなので、2つ目の引数を取得する
    if (path == NULL || *path == '\0')  // 引数がない場合は、ホームディレクトリに移動する
    {
        path = getenv("HOME");
        if (path == NULL)
        {
            printf("cd: HOME not set\n");
            return;
        }
    }
    
    if (chdir(path) == -1)
    {
        printf("cd: %s: %s\n", path, strerror(errno));
        return;

    }
    // exit(0);
}

// // minishellに接続しないで動作確認するためのコード 
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// void mini_cd(const char *path)   // テスト用に文字列リテラルを想定
// {
//     if (path == NULL || *path == '\0')
//     {
//         path = getenv("HOME");
//         if (path == NULL)
//         {
//             printf("cd: HOME not set\n");
//             return;
//         }
//     }
//     if (chdir(path) == -1)
//     {
//         printf("cd: %s: %s\n", path, strerror(errno));
//         return;
//     }
//     return;
// }

// int main(void)
// {
//     // プロセスが終了するとディレクトリ変更の影響がなくなるため、
//     // 挙動をテストするには、
//     // system("pwd")で現在のディレクトリを表示させる。
//     mini_cd("..");
//     system("pwd");
//     mini_cd("");
//     system("pwd");
//     mini_cd("Desktop/");
//     system("pwd");
//     return (0);
// }