#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft/libft.h"

// 実装できないシステムコール関係のエラー
void fatal_error(char *message)
{
    ft_putendl_fd(message, STDERR_FILENO);
    exit(EXIT_FAILURE);
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
    path = (char *)malloc(sizeof(char)*(i) + 2);
    if (path == NULL)
    {
        fatal_error("Malloc Error");
    }
    ft_strlcpy(path, paths, i+1);
    ft_strlcat(path, "/", i+2);
    return path;
}

char *search_path(char *filename)
{
    extern char **environ;
    char *paths = getenv("PATH");
    char *path;
    char *executable;
    
    path = NULL;
    executable = NULL;
    while(*paths)
    {
        path = trim_single_path(paths);
        paths = ft_strchr(paths, ':');
        if (paths != NULL)
        {
            paths++;
        }
        executable = ft_strjoin(path, filename);
        if (executable == NULL)
        {
            fatal_error("Malloc Error");
        }
        if (access(executable, F_OK) == 0)
        {
            free(path);
            return (executable);
        }
        else
        {
            free(executable);
            free(path);
        }
        if (paths == NULL)  // 環境変数内の全てのパスを見終わった後にbreakしてなかったから
            break;          // 次のループに入った時にNULLのポインタにファイルを繋ぎ合わせようとしてたことがセグフォの原因
    }
    return (NULL);
}

int main(void)
{
    // char *p = "pwd";
    char *p2 = "abc";

    // printf("%s\n", search_path(p));
    printf("%s\n", search_path(p2));
    return (0);
}