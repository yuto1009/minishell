/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:27 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/17 12:42:47 by yutoendo         ###   ########.fr       */
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
    
    value = getenv("PATH"); // $PATH変数にある、コロンで連結された複数のパスを取得する
    while (*value)
    {
        ft_bzero(path, PATH_MAX);   // 現在のパスが実行可能か確認するために確保
        end = ft_strchr(value, ':');    // $PATH変数にある複数のパスの最初を区切る
        if (end)
            strncpy(path, value, end-value);    // コロンが見つかったら、パスをコピーする
        else
            ft_strlcpy(path, value, PATH_MAX);  // コロンがない、つまり$PATH変数内の最後のパスの場合（やることは上と同じ）
        ft_strlcat(path, "/", PATH_MAX);    // 現在のパスの最後にバックスラッシュを置く
        ft_strlcat(path, filename, PATH_MAX);   // バックスラッシュを置いた後に、ファイル名(ex:pwd, ls)を置く
        if (access(path, X_OK) == 0)    // 上の流れで作成したパスに実行可能ファイルがあるか確認
        {
            char *dup;

            dup = ft_strdup(path);
            if (dup == NULL)
                fatal_error("strdup");
            return dup; // 実行可能なパスを返す(ex: /bin/ls, /bin/pwd)
        }
        if (end == NULL)
            return NULL;
        value = end+1;  // ポインタを次のパスに合わせる
    }
    return NULL;    // $PATH内に実行可能ファイルがなかったらNULLを返す
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

    pid = fork();   // 子プロセスを作成
    if (pid < 0)
        fatal_error("fork");
    else if (pid == 0)  // 子プロセスの場合
    {
        if (ft_strchr(path, '/') == NULL)
            path = search_path(path);
        validate_access(path, argv[0]);
        execve(path, argv, environ);
        fatal_error("execve");
    }
    else    // 親プロセスの場合
    {
        wait(&wstatus); // 子プロセスの終了を待機
        return(WEXITSTATUS(wstatus));   // 子プロセスの終了ステータスを返す
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