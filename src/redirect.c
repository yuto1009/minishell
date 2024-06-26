/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:20:06 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/05/18 18:37:46 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	dup_fd(t_node *node)
{
	int	fileoutfd;
	int	fileinfd;

	fileoutfd = node->redirout_fd;
	fileinfd = node->redirin_fd;
	if (fileoutfd != 1)
	{
		node->currentout_fd = stashfd(STDOUT_FILENO);
		dup2(fileoutfd, STDOUT_FILENO);
		close(fileoutfd);
	}
	if (fileinfd != 0)
	{
		node->currentin_fd = stashfd(STDIN_FILENO);
		dup2(fileinfd, STDIN_FILENO);
		close(fileinfd);
	}
}

void	reset_fd(t_node *node)
{
	if (node->currentout_fd != STDOUT_FILENO)
	{
		dup2(node->currentout_fd, STDOUT_FILENO);
		close(node->currentout_fd);
	}
	if (node->currentin_fd != STDIN_FILENO)
	{
		dup2(node->currentin_fd, STDIN_FILENO);
		close(node->currentin_fd);
	}
}

int	stashfd(int fd)
{
	int	stashfd;

	stashfd = dup(fd);
	if (stashfd < 0)
		fatal_error("dup error : stashfd failed\n");
	return (stashfd);
}

static int	open_file(t_node *node, int is_builtin)
{
	char	*filename;

	filename = node->token->next->str;
	if (ft_strncmp(node->token->str, ">>", 2) == 0)
		node->redirout_fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (ft_strncmp(node->token->str, "<<", 2) == 0)
		node->redirin_fd = heredoc(filename);
	else if (ft_strncmp(node->token->str, ">", 1) == 0)
		node->redirout_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (ft_strncmp(node->token->str, "<", 1) == 0)
		node->redirin_fd = open(filename, O_RDONLY);
	if (node->redirin_fd < 0)
	{
		if (is_builtin == 1)
			return (return_error("No such file or directory", filename, node));
		cmd_error_exit(filename, "No such file or directory", 1);
	}
	if (node->redirout_fd < 0)
		cmd_error_exit(filename, "Permission denied", 1);
	return (0);
}

char	**search_redir(t_node *node, int len, int is_builtin)
{
	int		i;
	char	**token2argv;
	t_node	*tmp;
	t_token	*current_token;

	i = 0;
	token2argv = (char **)ft_calloc(len + 1, sizeof(char *));
	tmp = node;
	current_token = node->token;
	while (token2argv != NULL && tmp->token->kind != TK_EOF)
	{
		if (tmp->token->kind == TK_REDIRECTION)
		{
			if (open_file(tmp, is_builtin) == 1)
				return (free(token2argv), node->token = current_token, NULL);
			tmp->token = tmp->token->next;
		}
		else
		{
			token2argv[i] = tmp->token->str;
			i++;
		}
		tmp->token = tmp->token->next;
	}
	return (node->token = current_token, token2argv);
}
