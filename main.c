/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:27 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/16 14:33:15 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int main()
// {
//     char *line = NULL;

//     while (1)
//     {
//         line = readline("> ");
//         if (line == NULL || strlen(line) == 0)
//         {
//             free(line);
//             break;
//         }
//         if(strcmp(line, "exit") == 0)
//         {
//             free(line);
//             break;
//         }
//         add_history(line);
//         printf("line is '%s'\n", line);
//         free(line);
        
//     }
//     printf("exit\n");
//     return 0;
// }

// void	sigaction_handler(int signal, siginfo_t *info, void *ucontext)
// {
//     (void)info;
// 	(void)ucontext;
//     if (signal == SIGINT)
//     {
//         // 次の行にプロンプトを表示する
//     } 
//     else if (signal == SIGQUIT)
//     {
//         // シェルを終了する
//     }
// }



// int main() {
//     struct sigaction sa;
//     char* line;
    
//     sa.sa_sigaction = sigaction_handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_SIGINFO;
//     sigaction(SIGINT, &sa, NULL);   // キーボードからの割り込みシグナル (ctl-c)
//     sigaction(SIGQUIT, &sa, NULL);   // キーボードによる中止シグナル (ctl-\)
    
//     while ((line = readline("> "))) { /* 行の読み取り */
//         printf("line is '%s'\n", line);
//         if (strlen(line) > 0) {
//             add_history(line); /* 履歴を保存 */
//         }
//         free(line);
//     }
    
//     printf("exit\n"); // EOFはctrl-dの終了を意味する
//     return 0;
// }

void fatal_error(const char *msg)
{
    dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

int interpret(char *line)
{
    extern char **environ;
    char *argv[] = {line, NULL};
    pid_t pid;
    int wstatus;

    pid = fork();
    if (pid > 0)
        fatal_error("fork");
    else if (pid == 0)
    {
        execve(line, argv, environ);
        fatal_error("execve");
    }
    else
    {
        wait(&wstatus);
        return(WEXITSTATUS(wstatus));
    }
}

int main(void)
{
    char *line;
    
    rl_outstream = stderr;
    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        free(line);
    }
    exit(EXIT_SUCCESS);
}   