/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:45:51 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/02/08 13:53:22 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
// int keep_fd(int fd)
// {
//     int keep_fd;
//     keep_fd = fcntl(fd, F_DUPFD, 10);
//     if (keep_fd < 0)
//         fatal_error("fcntl error");
//     return (keep_fd);
// }

// void redirect(int fd ,char *filename)
// {
//     int filefd;
//     int stashed_fd;
    
//     filefd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
//     filefd = keep_fd(filefd);
    
//     if (filefd < 0)
//     {
//         fatal_error("open error");
//     }
//     dup2(filefd, fd);
//     close(filefd);
// }



#include "../include/minishell.h"

int	stashfd(int fd)
{
	int	stashfd;

	stashfd = fcntl(fd, F_DUPFD, 10);
	// if (stashfd < 0)
	// 	fatal_error("fcntl");
	// if (close(fd) < 0)
	// 	fatal_error("close");
	return (stashfd);
}


void redirect_input(char *command ,char *filename)
{
	extern char **environ;
	int filefd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if(command)
		;
	dup2(filefd,0);
	char *argv[2];
	argv[0] = "cat";
	argv[1]=NULL;
	execve("/bin/cat",argv , environ);
}

void redirect_output(t_node *node, char **token2argv)
{
	int filefd, stashed_targetfd;
    char *argv[3];
    extern char **environ;
	// 1. Redirect先のfdをopenする
	// filefd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    filefd = node->redir_fd;
	filefd = stashfd(filefd); // filefdを退避させる

	// 2. Redirectする
	stashed_targetfd = stashfd(node->current_fd); // targetfdを退避させる
	if (filefd != node->current_fd)
	{
		dup2(filefd, node->current_fd); // filefdをtargetfdに複製する（元々のtargetfdは閉じられる）
		close(filefd);
	}
	execute(token2argv);
	// 3. コマンドを実行する
    // argv[0] = "echo";
    // argv[1] = "hello";
    // argv[2] = NULL;
    // execve("/bin/echo" , argv,environ);
	// 4. Redirectしていたのを元に戻す
	dup2(stashed_targetfd, node->current_fd); // 退避していたstashed_targetfdをtargetfdに複製する（元々のtargetfd）
}

int main(void)
{
    // redirect_output(1,"text.txt");
	redirect_input("cat","text.txt");
    return 0;
}