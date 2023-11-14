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
int main(int argc , char *argv[])
{
    int fd;
    fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fd, 1);
    close(fd);// fdはもう使わないので閉じる
    printf("Hello World\n");
    return 0;
}