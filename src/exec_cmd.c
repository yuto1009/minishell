/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:36:33 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/06/01 17:36:58 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_pipe(char **argv, t_var *env_map)
{
	extern char	**environ;
	char		*executable;

	if (ft_strchr(argv[0], '/') == NULL)
	{
		executable = search_path(argv[0], env_map);
		if (access(executable, F_OK) != 0 && !is_env_exists(env_map, "PATH"))
			cmd_error_exit(argv[0], "No such file or directory", 127);
	}
	else
	{
		executable = argv[0];
		if (access(executable, F_OK) != 0)
			cmd_error_exit(executable, "No such file or directory", 127);
	}
	check_access(argv[0]);
	execve(executable, argv, envlist2char(env_map));
	cmd_error_exit(argv[0], "command not found", 127);
}

bool	is_builtin(char *str)
{
	if (str && ft_strncmp(str, "echo", 5) == 0)
		return (true);
	else if (str && ft_strncmp(str, "cd", 3) == 0)
		return (true);
	else if (str && ft_strncmp(str, "pwd", 4) == 0)
		return (true);
	else if (str && ft_strncmp(str, "export", 7) == 0)
		return (true);
	else if (str && ft_strncmp(str, "unset", 6) == 0)
		return (true);
	else if (str && ft_strncmp(str, "env", 4) == 0)
		return (true);
	else if (str && ft_strncmp(str, "exit", 5) == 0)
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

void	handler(int signum)
{
	if (signum == SIGQUIT)
		printf("Quit: 3\n");
	else if (signum == SIGINT)
		printf("\n");
	rl_redisplay();
}
