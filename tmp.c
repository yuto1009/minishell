#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "libft/libft.h"
# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

//リダイレクト関数
// int main(int argc , char *argv[])
// {
//     int fd;
//     fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     int result = dup2(fd, 1);
//     close(fd);// fdはもう使わないので閉じる
//     printf("Hello World\n");
//     printf("Goodby, 42\n");
//     printf("return value: %d\n", result);
//     return 0;
// }

int main(void)
{
    int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int result = dup(fd);
    close(fd);
    printf("Hello World\n");
    printf("fd: %d, return value: %d\n", fd, result);

    return (0);
}