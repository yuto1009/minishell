/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 11:51:41 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/28 17:06:07 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_listsize(t_var *env_map)
{
	int		count;
	t_var	*tmp;

	count = 0;
	tmp = env_map;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		count++;
	}
	return (count);
}

char	*list2join(t_var *map)
{
	char	*namejoin;
	char	*alljoin;

	namejoin = ft_strjoin(map->name, "=");
	if (!namejoin)
	{
		free(namejoin);
		minishell_error("mallocerror");
	}
	alljoin = ft_strjoin(namejoin, map->value);
	if (!alljoin)
	{
		free(alljoin);
		minishell_error("mallocerror");
	}
	free(namejoin);
	return (alljoin);
}

char	**envlist2char(t_var *env_map)
{
	char	**ans;
	t_var	*tmp;
	int		i;
	char	*joinrow;

	ans = (char **)malloc(sizeof(char *) * get_listsize(env_map) + 1);
	if (!ans)
		return (NULL);
	tmp = env_map;
	i = 0;
	while (tmp != NULL)
	{
		joinrow = list2join(tmp);
		ans[i] = joinrow;
		i++;
		tmp = tmp->next;
	}
	ans[i] = NULL;
	return (ans);
}

void	check_access(char *args)
{
	struct stat	st;
	int			result;

	result = stat(args, &st);
	if (result)
		;
	if (ft_strncmp(args, "./", 2) == 0 && access(args, F_OK) != 0)
		cmd_error_exit(args, "NO such file or directory", 127);
	if (ft_strncmp(args, "./", 2) == 0 && access(args, X_OK) != 0)
		cmd_error_exit(args, "Permission denied", 126);
	if (ft_strncmp(args, "./", 2) == 0 && (st.st_mode & S_IFMT) == S_IFDIR)
		cmd_error_exit(args, "is a Directry", 126);
}
