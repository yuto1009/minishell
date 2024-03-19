/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:20:06 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/19 18:29:06 by yoshidakazu      ###   ########.fr       */
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
    extern char **environ;
    fileoutfd = node->redirout_fd;
    fileinfd = node->redirin_fd;
    if (fileoutfd != 1)
    {
        node->currentout_fd = stashfd(STDOUT_FILENO);
        dup2(fileoutfd, STDOUT_FILENO); // filefdをtargetfdに複製する（元々のtargetfdは閉じられる）
        close(fileoutfd);
    }
    if(fileinfd != 0)
    {
        node->currentin_fd = stashfd(STDIN_FILENO);
        dup2(fileinfd, STDIN_FILENO);
        close(fileinfd);
    }
}
void reset_fd(t_node *node)
{
    if(node->currentout_fd != STDOUT_FILENO)
    {
        dup2(node->currentout_fd,STDOUT_FILENO);
        close(node->currentout_fd);
    }
    if(node->currentin_fd != STDIN_FILENO)
    {
        dup2(node->currentin_fd,STDIN_FILENO);
        close(node->currentin_fd);
    }
}
char **serch_redir(t_node *node,int len)
{
    int i;
    // t_node *tmp;
    i = 0;
    char **token2argv;
    // tmp = node;
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
    // free(node);
    return token2argv;
}
