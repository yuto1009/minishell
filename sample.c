static char	*get_target_path(char **args, t_var *env_map)
{
	char	*home;
	char	*path;

	if (args[1] == NULL || ft_strncmp(args[1], "~", 2) == 0
		|| ft_strncmp(args[1], "~\\", 2) == 0)
	{
		home = get_env_value("HOME", env_map);
		if (!home)
			return (NULL); 
		path = (char *)malloc(sizeof(char) * ft_strlen(home) + 1);
		ft_strlcpy(path, home, ft_strlen(home) + 1);
		free(home);
	}
	else
	{
		path = (char *)malloc(sizeof(char) * ft_strlen(args[1]) + 1);
		ft_strlcpy(path, args[1], ft_strlen(args[1]) + 1);
	}
	return (path);
}

static int	change_directory_and_update_env(char *path, t_var *env_map,
		char *prev_pwd)
{
	char	*current_pwd;
	char	*old_pwd;

	current_pwd = ft_strdup("PWD");
	old_pwd = ft_strdup("OLDPWD");
	if (chdir(path) < 0)
	{
		free(current_pwd);
		free(old_pwd);
		return (cd_error(path));
	}
	unset_env(old_pwd, env_map);
	export_env(env_map, old_pwd, prev_pwd);
	unset_env(current_pwd, env_map);
	export_env(env_map, current_pwd, new_pwd(prev_pwd, path));
	free(current_pwd);
	free(old_pwd);
	return (0);
}

int	builtin_cd(char **args, t_var *env_map)
{
	char	*prev_pwd;
	char	*path;
	int		result;

	prev_pwd = get_env_value(ft_strdup("PWD"), env_map);
	path = get_target_path(args, env_map);
	if (!path)
		return (minishell_error("HOME not set or path allocation failed"));
	result = change_directory_and_update_env(path, env_map, prev_pwd);
	free(path);
	return (result);
}

// int	builtin_cd(char **args, t_var *env_map)
// {
// 	char	*home;
// 	char	*path;
// 	char	*prev_pwd;
//     char    *current_pwd;
//     char    *old_pwd;

//     current_pwd = ft_strdup("PWD");
//     old_pwd = ft_strdup("OLDPWD");
// 	prev_pwd = get_env_value(current_pwd, env_map);
// 	unset_env(old_pwd, env_map);
// 	export_env(env_map, old_pwd, prev_pwd);
// 	if (args[1] == NULL || ft_strncmp(args[1],"~",2) == 0 || ft_strncmp(args[1],"~\\",2) == 0 )
// 	{
// 		home = get_env_value("HOME", env_map);
// 		path = (char *)malloc(sizeof(char) * ft_strlen(home) + 1);
// 		if (!home)
// 			return (minishell_error("HOME not set"));
// 		ft_strlcpy(path, home, ft_strlen(home) + 1);
//         free(home);
// 	}
// 	else
// 	{
// 		path = (char *)malloc(sizeof(char) * ft_strlen(args[1]) + 1);
// 		ft_strlcpy(path, args[1], ft_strlen(args[1]) + 1);
// 	}
// 	if (chdir(path) < 0)
//     {
//         free(current_pwd);
// 		return (cd_error(path));
//     }
// 	unset_env(current_pwd, env_map);
// 	export_env(env_map, current_pwd, new_pwd(prev_pwd, path));
//     free(path);
// 	return (0);
// }