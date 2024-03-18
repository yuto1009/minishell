/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:36:33 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/18 10:59:30 by yoshidakazu      ###   ########.fr       */
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
        // printf("tmp : %s\n",tmp->name);
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

static bool is_buildin(char  *str)
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
	return (false);
}
static void exec_buildin(t_token *token,t_var *env_map)
{
    
        if(strcmp(token->str,"export") == 0){
            builtin_export(token,env_map);
            return ;
        }
        if(strcmp(token->str,"env") == 0){
            builtin_env(env_map);
            return ;
        }
        if(strcmp(token->str,"unset") == 0){
            builtin_unset(token,env_map);
            return ;
        }
}

int exec(t_node *node, t_var *env_map)
{
    char **token2argv;
    int len;
    int pid;
    while(node != NULL)
    { 
        len = count_token_len(node->token);
        set_pipe(node);
        if(is_buildin(node->token->str))
            exec_buildin(node->token,env_map);
        else
        {
            pid = fork();
            if(pid < 0)
                cmd_error_exit("fork","fork error",1);
            else if (pid == 0){
                token2argv= serch_redir(node,len);
                if(!token2argv)
                    exit(1);
                dup_child_pipe(node);
                dup_fd(node);
                execute_pipe(token2argv,env_map);
            }
            set_parent_pipe(node);
        }
        node = node->next;
    }
    return pid;
}