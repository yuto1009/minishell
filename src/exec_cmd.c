/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:36:33 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/20 10:01:03 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
//execで使用中


int get_listsize(t_var *env_map)
{
    int count;
    count = 0;
    t_var *tmp;
    tmp = env_map;
    while(tmp!=NULL)
    {
        tmp = tmp->next;
        count++;
    }
    return count;
}


char *list2join(t_var *map)
{
    char *namejoin;
    char *alljoin;
    namejoin = ft_strjoin(map->name,"=");
    if(!namejoin)
    {
        free(namejoin);
        minishell_error("mallocerror");
    }
    alljoin = ft_strjoin(namejoin,map->value);
    if(!alljoin)
    {
        free(alljoin);
        minishell_error("mallocerror");
    }
    free(namejoin);
    return alljoin;
}

static char **envlist2char(t_var *env_map)
{
    char **ans;
    t_var *tmp;
    int i;
    char *joinrow;

    ans = (char **)malloc(sizeof(char *) * get_listsize(env_map)+1);
    if(!ans)
        return NULL;
    tmp = env_map;
    i=0;
    while(tmp != NULL)
    {
        joinrow = list2join(tmp);
        ans[i] = joinrow;
        i++;
        tmp = tmp->next;
        
    }
    ans[i] =NULL;
    return ans;
}


static void execute_pipe(char **argv,t_var *env_map)
{
    extern char **environ;
    char *executable;
        if (ft_strchr(argv[0], '/') == NULL)
            executable = search_path(argv[0]);
        else
            executable = argv[0];
        execve(executable, argv, envlist2char(env_map));
        cmd_error_exit(argv[0], "command not found", 127);
}
// envlist2char(env_map)

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

 bool is_buildin(char  *str)
{
    if (ft_strncmp(str, "echo", 5) == 0)
		return (true);
	else if (ft_strncmp(str, "cd", 3) == 0)
		return (true);
	else if (ft_strncmp(str, "pwd", 4) == 0)
		return (true);
	else if (ft_strncmp(str, "export", 7) == 0)
		return (true);
	else if (ft_strncmp(str, "unset", 6) == 0)
		return (true);
	else if (ft_strncmp(str, "env", 4) == 0)
		return (true);
    else if(ft_strncmp(str, "exit", 5) == 0)
        return (true);
	return (false);
}

 int exec_buildin(char  **argv,t_var *env_map,int prev_status)
{
    
        if(strcmp(argv[0],"export") == 0){
            builtin_export(argv,env_map);
            return 0;
        }
        if(strcmp(argv[0],"env") == 0){
            builtin_env(env_map);
            return 0;
        }
        if(strcmp(argv[0],"unset") == 0)
            return builtin_unset(argv,env_map);
        if(strcmp(argv[0] , "echo") == 0)
            return builtin_echo(argv);
        if(strcmp(argv[0] , "pwd") == 0)
            return builtin_pwd();
        if(strcmp(argv[0],"exit") == 0)
            return builtin_exit(argv,prev_status);
        if(strcmp(argv[0],"cd") == 0)
            return builtin_cd(argv,env_map);
        return 0;
}

int exec(t_node *node, t_var *env_map,int prev_status)
{
    int pid;
        char **token2argv;

    while(node != NULL)
    { 
        set_pipe(node);
            pid = fork();
            if(pid < 0)
                cmd_error_exit("fork","fork error",1);
            else if (pid == 0)
            {
                dup_child_pipe(node);
                token2argv= serch_redir(node,count_token_len(node->token));
                if(!token2argv)
                    exit(1);
                dup_fd(node);
                if(is_buildin(token2argv[0]))
                    exit(exec_buildin(token2argv,env_map,prev_status));
                else
                {
                    execute_pipe(token2argv,env_map);
                }
            }
            set_parent_pipe(node);
        node = node->next;
    }
    return pid;
}
