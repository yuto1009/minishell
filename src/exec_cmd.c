/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:36:33 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/17 15:53:03 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
//execで使用中
int count_token_len(t_token *token)
{
    int i;

    i = 0;
    while(token!= NULL)
    {
        i++;
        token = token->next;
    }
    return (i-1);
}

void execute_pipe(char **argv)
{
    extern char **environ;
    char *executable;
        if (ft_strchr(argv[0], '/') == NULL)
            executable = search_path(argv[0]);
        else
            executable = argv[0];
        execve(executable, argv, environ);
        cmd_error_exit(argv[0], "command not found", 127);
}


 int serch_endindex(t_node *node)
{
    t_node *tmp;
    int count;
    tmp = node;
    count = 0;
    while(tmp!=NULL)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}

int exec(t_node *node)
{
    char **token2argv;
    int len;
    int pid;
    while(node != NULL)
    { 
        len = count_token_len(node->token);
        set_pipe(node);
        pid = fork();
        if(pid < 0)
            cmd_error_exit("fork","fork error",1);
        else if (pid == 0){
            token2argv= serch_redir(node,len);
            if(!token2argv)
                exit(1);
            dup_child_pipe(node);
            dup_fd(node);
            execute_pipe(token2argv);
        }
        set_parent_pipe(node);
        node = node->next;
    }
    return pid;
}