/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/31 11:19:33 by yutoendo         ###   ########.fr       */
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

void error_exit(char *message, int exit_status)
{
    ft_putendl_fd(message, STDERR_FILENO);
    exit(exit_status);
}

bool is_path_executable(const char *path)
{
    int result = access(path, X_OK);
    if (result < 0)
    {
        return (false);
    }
    return (true);
}

char *trim_single_path(char *paths)
{
    size_t i;
    char *path;

    i = 0;
    while(paths[i] != '\0')
    {
        if (paths[i] == ':')
            break;
        i++;
    }
    path = (char *)malloc(sizeof(char)*(i) + 1);
    if (path == NULL)
        fatal_error("Malloc Error");
    ft_strlcpy(path, paths, i+1);
    return path;
}

char *search_path(char *filename)
{
    extern char **environ;
    char *paths = getenv("PATH");
    char *path;
    
    path = NULL;
    while(*paths)
    {
        path = trim_single_path(paths);
        paths = ft_strchr(paths, ':');
        if (paths == NULL && is_path_executable(ft_strjoin(path, filename)))
            break;
        else if(paths == NULL)
        {
            assert_error()
        }
        paths++;
        if (is_path_executable(ft_strjoin(path, filename)))
        {
            break;
        }
        // printf("%s\n", path);
        free(path);
    }
    return (path);
}

int interpret(char *line)
{
    pid_t pid = fork();
    char *path;
    char *argv[] = {line, NULL};
    extern char **environ;
    int wstatus;
    
    if (pid < 0)
        fatal_error("fork");
    if (pid == 0)
    {
        // // 子プロセス
        // if (is_path_executable(line))
        // {
        //     execve(line, argv, environ);
        // }
        // else
        // {
        //     assert_error("command not found");
        // }
        if (ft_strchr(line, '/') == NULL)
        {
            path = search_path(line);
        }
        else
        {
            path = line;
        }
        if (is_path_executable(path))
        {
            const char *argv[] = {path, NULL};
            execve(path, argv, environ);
        }
        else
        {
            assert_error("Unknown Path");
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