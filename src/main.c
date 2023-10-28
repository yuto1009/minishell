/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/28 17:08:52 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Readlineの出力先をstderrに変更 
Macに標準装備のReadlineはデフォルトで出力先がstderrになっているが、
GNUのreadlineがはstdoutになっているため、変更が必要。
rl_outstreamはGNU Readlineライブラリ内の変数 */
void set_output_destination(FILE *dst)
{
    rl_outstream = dst;
}

void fatal_error(char *message)
{
    ft_putendl_fd(message, STDERR_FILENO);
    exit(EXIT_FAILURE);
}

void assert_error(char *message)
{
    ft_putendl_fd(message, STDERR_FILENO);
}

bool is_path_executable(const char *filename)
{
    int result = access(filename, X_OK);
    if (result < 0)
    {
        return (false);
    }
    return (true);
}

int interpret(char *line)
{
    pid_t pid = fork();
    char *argv[] = {line, NULL};
    extern char **environ;
    int wstatus;
    
    if (pid < 0)
        fatal_error("fork");
    if (pid == 0)
    {
        // 子プロセス
        if (is_path_executable(line))
        {
            execve(line, argv, environ);
        }
        else
        {
            assert_error("command not found");
        }
    }
    else 
    {
        // 親プロセス
        wait(&wstatus);
    }
    return (WEXITSTATUS(wstatus));
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }

int main(void)
{
    char *line;
    int status;

    set_output_destination(stderr);
    status = 0;
    while(1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;  // breakをreturn (0)に変えるとリークが確認できる (テスターがNG出すようになる)
        add_history(line);
        status = interpret(line);
        free(line);
    }
    return (status);
}