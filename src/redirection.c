/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 13:45:51 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/01/26 18:04:00 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int keep_fd(int fd)
{
    int keep_fd;
    keep_fd = fcntl(fd, F_DUPFD, 10);
    if (keep_fd < 0)
        fatal_error("fcntl error");
    return (keep_fd);
}

void redirect(int fd ,char *filename)
{
    int filefd;
    int stashed_fd;
    
    filefd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    filefd = keep_fd(filefd);
    
    if (filefd < 0)
    {
        fatal_error("open error");
    }
    dup2(filefd, fd);
    close(filefd);
}

int main(void)
{
    redirect(1,"text.txt");
}