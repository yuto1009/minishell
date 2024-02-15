/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:20:54 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/02/15 18:29:10 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
 
#include <err.h>
#include <errno.h>
// int main()
// {
//     char *argv[] = {"ls", "|", "head" , "|", "wc", NULL};
//     int i, pipe_locate[10], pipe_count = 0;
//     pipe_locate[0] = -1;
//     for (i = 0; argv[i] != NULL; i++) {
//         if (strcmp(argv[i], "|") == 0) {
//             pipe_count++;
//             pipe_locate[pipe_count] = i;
//             argv[i] = NULL;
//         }
//     }

//     int pfd[9][2];
//     if (pipe_count == 0) {
//         if (fork() == 0) {
//             execvp(argv[0], argv);
//             exit(0);
//         }
//         else {
//             int status;
//             wait(&status);
//         }
//     }

//     for (i = 0; i < pipe_count + 1 && pipe_count != 0; i++) {
//         if (i != pipe_count)
//             pipe(pfd[i]);

//         if (fork() == 0) {
//             if (i == 0) {//初手パイプ
//                 dup2(pfd[i][1], 1);
//                 close(pfd[i][0]);
//                 close(pfd[i][1]);
//             } else if (i == pipe_count) {//last pipe 
//                 dup2(pfd[i - 1][0], 0);
//                 close(pfd[i - 1][0]);
//                 close(pfd[i - 1][1]);
//             } else {
//                 dup2(pfd[i - 1][0], 0);
//                 dup2(pfd[i][1], 1);
//                 close(pfd[i - 1][0]);
//                 close(pfd[i - 1][1]);
//                 close(pfd[i][0]);
//                 close(pfd[i][1]);
//             }

//             execvp(argv[pipe_locate[i] + 1], argv + pipe_locate[i] + 1);
//             exit(0);
//         }
//         else if (i > 0) {
//             printf("ok\n");
//             close(pfd[i - 1][0]); close(pfd[i - 1][1]);
//         }
//     }
//     int status;

//     for (i = 0; i < pipe_count + 1; i++) {
//         wait(&status);
//     }
//     return 0;
// }
 
    extern char **environ;
int exec(char *argv1[])
{
    int pipfd[2];
     pipe(pipfd);
  int status;
    if (fork() == 0) {
        close(pipfd[0]); // パイプの読み込み端を閉じる
        dup2(pipfd[1], 1);
        close(pipfd[1]); // データを書き込んだ後は不要なので閉じる
        execve("/bin/cat", argv1, environ);
        // エラー処理
    }
    wait(&status); // 2番目の子プロセスの終了を待つ
    return pipfd[0];
    // printf("%d\n",pipfd[0]);
}
int main(void)
{

    char *argv1[] = {"cat", "hello.txt", NULL};
    char *argv2[] = {"wc", "-l", NULL};
    
    extern char **environ;
    int status;
    // int currentpfd = 0;
   int infd = 0;
    infd = exec(argv1);   
        // printf("%d\n",pipfd[0]);

    if (fork() == 0) {
        // close(pipfd[1]); // パイプの書き込み端を閉じる
        dup2(infd, 0);
        close(infd); // データを読み込んだ後は不要なので閉じる
        execve("/usr/bin/wc", argv2, environ);
        // エラー処理
    }

    // close(pipfd[0]); // 親プロセスではパイプを使用しないため閉じる
    // close(pipfd[1]); // 親プロセスではパイプを使用しないため閉じる
    wait(&status); // 最初の子プロセスの終了を待つ

    return 0;
}

// int main(void)
// {
//     char *argv1[] = {"grep","h", "hello.txt", NULL};
//      extern char **environ;
//     execve("/usr/bin/grep", argv1, environ);
// }