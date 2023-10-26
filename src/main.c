/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:27 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/26 14:58:14 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

pid_t exec_pipeline(t_node *node)
{
    extern char **environ;
    char *path;
    pid_t pid;
    char **argv;

    if (node == NULL)
    {
        return (-1);                
    }
    prepare_pipe(node);
    pid = fork();   // 子プロセスを作成
    if (pid < 0)
        fatal_error("fork");
    else if (pid == 0)  // 子プロセスの場合
    {
        // argv = token_list_to_argv(node->args);
        prepare_pipe_child(node);
        do_redirect(node->command->redirects);
        argv = token_list_to_argv(node->command->args);
        path = argv[0];
        if (ft_strchr(path, '/') == NULL)   // pwd ls
            path = search_path(path);
        validate_access(path, argv[0]);
        execve(path, argv, environ);    // 実行される　現在のプロセスが新しいプロセスに置き換わる
        reset_redirect(node->command->redirects);
        fatal_error("execve");
    }
    // 親プロセス
    prepare_pipe_parent(node);
    if (node->next != NULL)
    {
        return (exec_pipeline(node->next));        
    }
    return (pid);
}

int wait_pipeline(pid_t last_pid)
{
    pid_t wait_result;
    int status;
    int wstatus;
    
    while (1)
    {
        wait_result = wait(&wstatus);
        if (wait_result == last_pid)
        {
            status = WEXITSTATUS(wstatus);    
        }
        else if (wait_result < 0)
        {
            if (errno == ECHILD)
                break;
        }
    }
    return (status);
}

int exec(t_node *node)
{
    pid_t last_pid;
    int status;
    
    if (open_redir_file(node) < 0)
    {
        return (ERROR_OPEN_REDIR);
    }
    last_pid = exec_pipeline(node);
    status = wait_pipeline(last_pid);
    return (status);
}

// 
void interpret(char *const line, int *status)
{   
    t_token *token;
    t_node *node;
    
    token = tokenize(line); // トークン化（字句解析）
    if (at_eof(token))
        ;
    else if (syntax_error)
        *status = ERROR_TOKENIZE;
    else
    {
        node = parse(token);    // 構文解析
        if (syntax_error)
        {
            *status = ERROR_PARSE;
        }
        else
        {
            expand(node);
            *status = exec(node);
        }
        free_node(node);
    }
    free_token(token);
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
        interpret(line, &status);
        free(line);
    }
    exit(status);
}
