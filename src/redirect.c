/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:00:51 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/26 14:06:55 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int stash_fd(int fd)
{
    int stash_fd;

    stash_fd = fcntl(fd, F_DUPFD, 10);
    if (stash_fd < 0)
        fatal_error("fcntl");
    if (close(fd) < 0)
        fatal_error("close");
    return (stash_fd);
}

int read_heredoc(const char *delimiter)
{
    char *line;
    int pfd[2];

    if (pipe(pfd) < 0)
    {
        fatal_error("pipe");            
    }
    while(1)
    {
        line = readline("> ");
        if (line == NULL)
            break;
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        dprintf(pfd[1], "%s\n", line);
        free(line);
    }
    close(pfd[1]);
    return (pfd[0]);
}

int open_redir_file(t_node *redir)
{
    if (redir == NULL)
        return (0);
    if (redir->kind == ND_REDIR_OUT)
    {
        redir->file_fd = open(redir->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    }
    else if (redir->kind == ND_REDIR_IN)
    {
        redir->file_fd = open(redir->filename->word, O_RDONLY);
    }
    else if (redir->kind == ND_REDIR_APPEND)
    {
        redir->file_fd = open(redir->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
    }
    else if (redir->kind == ND_REDIR_HEREDOC)
    {
        redir->file_fd = read_heredoc(redir->delimiter->word);            
    }
    else
    {
        assert_error("open_redir_file");
    }
    if (redir->file_fd < 0)
    {
        xperror(redir->filename->word);
        return (-1);
    }
    redir->file_fd = stash_fd(redir->file_fd);
    return (open_redir_file(redir->next));
}

bool is_redirect(t_node *node)
{
    if (node->kind == ND_REDIR_OUT)
        return (true);
    else if (node->kind == ND_REDIR_IN)
        return (true);
    else if (node->kind == ND_REDIR_APPEND)
        return (true);
    else if (node->kind == ND_REDIR_HEREDOC)
        return (true);
    return (false);
}

void do_redirect(t_node *redir)
{
    if (redir == NULL)
        return ;
    if (is_redirect(redir) == true)
    {
        redir->stashed_target_fd = stash_fd(redir->target_fd);
        dup2(redir->file_fd, redir->target_fd);
    }
    else
    {
        assert_error("do_redirect");
    }
    do_redirect(redir->next);
}

// Reset must be done from tail to head
void reset_redirect(t_node *redir)
{
    if (redir == NULL)
        return ;
    reset_redirect(redir->next);
    if (is_redirect(redir) == true)
    {
        close(redir->file_fd);
        close(redir->target_fd);
        dup2(redir->stashed_target_fd, redir->target_fd);
    }
    else
    {
        assert_error("reset_redirect");
    }
}