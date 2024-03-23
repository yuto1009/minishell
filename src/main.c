/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/23 14:50:35 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_token_len(t_token *token)
{
	int		i;
	t_token	*tmp;

	tmp = token;
	i = 0;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	free(tmp);
	return (i - 1);
}

// static t_token *interpret_line2token(char *line, t_var *env_map)
// {
//     t_token *token;
//     int status;
//     int prev_status;

//     prev_status = g_status;
//     g_status = 0;
//     token = tokenize(line);
//     if (expand(token, env_map, prev_status) < 0)
//         return ;
//     status = tokenize_error(token);
//     if (status == SYNTAX_ERROR || status == CMD_NOT_FOUND_ERROR)
//     {
//         free(token);
//         g_status = status;
//         return ;
//     }
    
// }

void	interpret(char *line, t_var *env_map)
{
	struct s_node	*node;
	int				status;
	int				prev_status;
	char			**token2argv;
	pid_t			pid;
	t_token			*token;

	prev_status = g_status;
	g_status = 0;
	token = tokenize(line);
	node = NULL;
	if (expand(token, env_map, prev_status) < 0)
		return ;
	status = tokenize_error(token);
	if (status == SYNTAX_ERROR || status == CMD_NOT_FOUND_ERROR)
	{
		free(token);
		g_status = status;
		return ;
	}
	node = parser(token);
	if (node->next == NULL && is_buildin(node->token->str))
	{
		token2argv = serch_redir(node, count_token_len(node->token));
		if (!token2argv)
			exit(1);
		dup_fd(node);
		g_status = exec_buildin(token2argv, env_map, prev_status);
		reset_fd(node);
	}
	else
	{
		pid = exec(node, env_map, prev_status);
		wait_pid(pid);
	}
	return ;
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }

bool	is_only_blank_character(char *line)
{
	bool	ans;

	ans = false;
	if (strcmp(line, "") == 0)
		return (true);
	while (is_blank(*line) && *line != '\0')
	{
		ans = true;
		line++;
	}
	if (*line != '\0')
		ans = false;
	return (ans);
}

void	roop_readline(void)
{
	t_var	*env_map;
	char	*line;

	env_map = init_env_map();
	set_output_destination(stderr);
	g_status = 0;
	while (1)
	{
		setup_signal();
		line = readline("minishell$ ");
		if (line == NULL)
		{
			break ;
		}
		if (is_only_blank_character(line))
			continue ;
		if (*line)
			add_history(line);
		interpret(line, env_map);
		free(line);
	}
	return ;
}

int	main(void)
{
	set_output_destination(stderr);
	roop_readline();
	printf("exit\n"); 
	return (g_status);
}
