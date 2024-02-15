/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:20:54 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/02/14 22:54:25 by yoshidakazu      ###   ########.fr       */
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

int main(void)
{
    t_node *node;
    node = (t_node*)calloc(1,sizeof(t_node));
    node->token = (t_token*)calloc(1,sizeof(t_token));
    node->token->str = "|";
    
    node->left = (t_node*)calloc(1,sizeof(t_node));
    node->left->token = (t_token*)calloc(1,sizeof(t_token));
    
    
    node->left->token->next = (t_token*)calloc(1,sizeof(t_token));
    node->left->token->next->next = (t_token*)calloc(1,sizeof(t_token));
    
    node->left->token->str = "cat";
    node->left->token->next->str = "hello.txt";
    node->left->token->next->next->str = NULL;
    
    node->right = (t_node*)calloc(1,sizeof(t_node));
    node->right->token = (t_token*)calloc(1,sizeof(t_token));
    node->right->token->next = (t_token*)calloc(1,sizeof(t_token));
    node->right->token->next->next = (t_token*)calloc(1,sizeof(t_token));
    
    node->right->token->str = "wc";
    node->right->token->next->str = "-l";
    node->right->token->next->next->str = NULL;
    printf("%s",node->token->str);
    // char *argv1[] = {"cat", "hello.txt", NULL};
    // char *argv2[] = {"wc", "-l", NULL};
    // int pipfd[2];
    // extern char **environ;
    // int status;
    // int currentpfd = 0;
    // pipe(pipfd);
if(strcmp(node->token->str,"|") == 0)
{
    
}
    // if (fork() == 0) {
    //     close(pipfd[0]); // パイプの読み込み端を閉じる
    //     dup2(pipfd[1], 1);
    //     close(pipfd[1]); // データを書き込んだ後は不要なので閉じる
    //     execve("/bin/cat", argv1, environ);
    //     // エラー処理
    // }

    // if (fork() == 0) {
    //     close(pipfd[1]); // パイプの書き込み端を閉じる
    //     dup2(pipfd[0], 0);
    //     close(pipfd[0]); // データを読み込んだ後は不要なので閉じる
    //     execve("/usr/bin/wc", argv2, environ);
    //     // エラー処理
    // }

    // close(pipfd[0]); // 親プロセスではパイプを使用しないため閉じる
    // close(pipfd[1]); // 親プロセスではパイプを使用しないため閉じる
    // wait(&status); // 最初の子プロセスの終了を待つ
    // wait(&status); // 2番目の子プロセスの終了を待つ

    return 0;
}

// int main(void)
// {
//     char *argv1[] = {"grep","h", "hello.txt", NULL};
//      extern char **environ;
//     execve("/usr/bin/grep", argv1, environ);
// }