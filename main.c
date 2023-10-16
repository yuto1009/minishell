/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:27 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/16 21:46:45 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fatal_error(const char *msg) __attribute__((noreturn));
void    err_exit(const char *location, const char *msg, int status) __attribute__((noreturn));

void fatal_error(const char *msg)
{
    dprintf(STDERR_FILENO, "Fatal Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

// int interpret(char *line)
// {
//     extern char **environ;
//     char *argv[] = {line, NULL};
//     pid_t pid;
//     int wstatus;

//     pid = fork();
//     if (pid < 0)
//         fatal_error("fork");
//     else if (pid == 0)
//     {
//         execve(line, argv, environ);
//         fatal_error("execve");
//     }
//     else
//     {
//         wait(&wstatus);
//         return(WEXITSTATUS(wstatus));
//     }
// }

void    err_exit(const char *location, const char *msg, int status)
{
    dprintf(STDERR_FILENO, "$minishell: %s %s\n", location, msg);
    exit(status);
}

char *search_path(const char *filename)
{
    char path[PATH_MAX];
    char *value;
    char *end;
    
    value = getenv("PATH");
    while (*value)
    {
        ft_bzero(path, PATH_MAX);
        end = ft_strchr(value, ':');
        if (end)
            strncpy(path, value, end-value);    // 作る
        else
            ft_strlcpy(path, value, PATH_MAX);
        ft_strlcat(path, "/", PATH_MAX);
        ft_strlcat(path, filename, PATH_MAX);
        if (access(path, X_OK) == 0)
        {
            char *dup;

            dup = ft_strdup(path);
            if (dup == NULL)
                fatal_error("strdup");
            return dup;
        }
        if (end == NULL)
            return NULL;
        value = end+1;
    }
    return NULL;
}

void validate_access(const char *path, const char *filename)
{
    if (path == NULL)
        err_exit(filename, "command not found", 127);
    if (access(path, F_OK) < 0)
        err_exit(filename, "command not found", 127);   
}

int exec(char *argv[])
{
    extern char **environ;
    const char *path = argv[0];
    pid_t pid;
    int wstatus;

    pid = fork();
    if (pid < 0)
        fatal_error("fork");
    else if (pid == 0)
    {
        if (ft_strchr(path, '/') == NULL)
            path = search_path(path);
        validate_access(path, argv[0]);
        execve(path, argv, environ);
        fatal_error("execve");
    }
    else
    {
        wait(&wstatus);
        return(WEXITSTATUS(wstatus));
    }
}

int interpret(char *const line)
{
    int status;
    char *argv[] = {line, NULL};
    
    status = exec(argv);
    return status;
}

int main(void)
{
    char *line;
    int status;
    
    rl_outstream = stderr;
    status = EXIT_SUCCESS;
    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        status = interpret(line);
        free(line);
    }
    exit(status);
}