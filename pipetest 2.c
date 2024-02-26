#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void execute_cmd(const char *cmd[], int input_fd, int output_fd) {
    if (fork() == 0) {
        if (input_fd != 0) {
            dup2(input_fd, 0);
            // close(input_fd);
        }
        if (output_fd != 1) {
            dup2(output_fd, 1);
            // close(output_fd);
        }
        execvp(cmd[0], (char *const *)cmd);
        perror("execvp");
        exit(1);
    }
}

int main(void) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // lsコマンドを実行する
    const char *ls_cmd[] = {"ls", NULL};
    execute_cmd(ls_cmd, 0, pipefd[1]);
    close(pipefd[1]); // 子プロセスが終了したら書き込み用のファイルディスクリプタを閉じる

    // wc -lコマンドを実行する
    const char *wc_cmd[] = {"wc", "-l", NULL};
    execute_cmd(wc_cmd, pipefd[0], 1);
    close(pipefd[0]); // 読み込み用のファイルディスクリプタを閉じる

    // 子プロセスが終了するのを待つ
    // for (int i = 0; i < 2; i++) {
    //     wait(NULL);
    // }
    if(node == head)
    
    else if(node == end)
    else
    return 0;
}
