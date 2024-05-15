/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:36:33 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/15 14:13:01 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	execute_pipe(char **argv, t_var *env_map)
{
	extern char	**environ;
	char		*executable;
	if (ft_strchr(argv[0], '/') == NULL)
		executable = search_path(argv[0]);
	else
		executable = argv[0];        
	check_access(argv[0]);
	execve(executable, argv, envlist2char(env_map));
    
	cmd_error_exit(argv[0], "command not found", 127);
}

bool	is_builtin(char *str)
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
	else if (ft_strncmp(str, "exit", 5) == 0)
		return (true);
	return (false);
}

int	exec_builtin(char **argv, t_var *env_map, int prev_status)
{
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (builtin_export(argv, env_map));
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (builtin_env(env_map));
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (builtin_unset(argv, env_map));
	else if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (builtin_echo(argv));
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (builtin_pwd(env_map));
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (builtin_exit(argv, prev_status));
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (builtin_cd(argv, env_map));
	return (0);
}

int	exec(t_node *node, t_var *env_map, int prev_status)
{
	int		pid;
	char	**token2argv;

	while (node != NULL)
	{
		set_pipe(node);
		pid = fork();
		if (pid < 0)
			return(cmd_error_return("fork", "fork error", 1));
		else if (pid == 0)
		{
			dup_child_pipe(node);
			token2argv = search_redir(node, count_token_len(node->token));
			if (!token2argv)
				exit(1);
			dup_fd(node);
			if (is_builtin(token2argv[0]))
				exit(exec_builtin(token2argv, env_map, prev_status));
			else
				execute_pipe(token2argv, env_map);
		}
		set_parent_pipe(node);
		node = node->next;
	}
	return (pid);
}
