/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:20:06 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/02/29 19:44:20 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
int stashfd(int fd)
{
    int stashfd;
    //fcntlは使用不可

    stashfd = fcntl(fd, F_DUPFD, 10);
    return (stashfd);
}

int heredoc(char *delimiter)
{
    char    *line;
    int     pfd[2];

        pipe(pfd);
        signal_heredoc();
    while (1)
    {
        line = readline("> ");
        if (line == NULL)
        {
            free(line);
            break ;
        }
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        while(*line != '\0') // tempを使って文字列を操作
        {
            write(pfd[1], line, 1);
            line++;
        }
        write(pfd[1], "\n", 1);
    }
    close(pfd[1]);

    return pfd[0];
}


void open_file(t_node *node)
{
    char *filename;

    filename = node->token->next->str;
    if(ft_strncmp(node->token->str , ">>",2) == 0)
        node->redirout_fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
    else if(ft_strncmp(node->token->str , "<<",2) == 0)
        node->redirin_fd  = heredoc(filename);
    else if(ft_strncmp(node->token->str , ">",1) == 0 )
        node->redirout_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if(ft_strncmp(node->token->str , "<",1) == 0)
        node->redirin_fd = open(filename,O_RDONLY);   
        if(node->redirin_fd < 0)
            cmd_error_exit(filename,"No such file or directory", 172);
    // if(node->redir_fd == -1)
        // todo("ファイルが存在しません");
    
        
    // return node;
}



void dup_fd(t_node *node)
{
    int fileoutfd,fileinfd;
    // int stashedout_targetfd = 0,stashedin_targetfd;
    extern char **environ;
    // 1. Redirect先のfdをopenする
    // fileoutfd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    fileoutfd = node->redirout_fd;
    fileinfd = node->redirin_fd;
    // fileoutfd = stashfd(fileoutfd); // filefdを退避させる
    // printf("fileinfd : %d\n fileoutfd : %d\n",fileinfd,fileoutfd);
    // 2. Redirectする
    // stashedin_targetfd = stashfd(node->currentin_fd); // targetfdを退避させる
    // stashedout_targetfd = stashfd(node->currentout_fd); // targetfdを退避させる
    if (fileoutfd != 1)
    {
        dup2(fileoutfd, node->currentout_fd); // filefdをtargetfdに複製する（元々のtargetfdは閉じられる）
        close(fileoutfd);
    }
    if(fileinfd != 0)
    {
        dup2(fileinfd, node->currentin_fd);
        close(fileinfd);
    }
}

char **serch_redir(t_node *node,int len)
{
    int i;
    i = 0;
    char **token2argv;
    token2argv = (char **)ft_calloc(len+1,sizeof(char *));
    if(!token2argv)
        return NULL;
while(node->token->kind !=TK_EOF)
    {
        if(node->token->kind == TK_REDIRECTION)
        {
            open_file(node);
            node->token = node->token->next;
        }
        else
        {
            token2argv[i] = node->token->str;
            i++;
        }
        node->token = node->token->next;
    }
    return token2argv;
}
