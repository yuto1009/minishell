/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:23:23 by yutoendo          #+#    #+#             */
/*   Updated: 2024/05/01 00:13:14 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_var	*create_map(char *name, char *value)
{
	t_var	*map;

	map = (t_var *)ft_calloc(1, sizeof(t_var));
	if (map == NULL)
		fatal_error("malloc error");
	map->name = name;
	map->value = value;
	map->prev = NULL;
	map->next = NULL;
	return (map);
}

t_var	*export_env(t_var *map, char *env_name, char *env_value)
{
	if (map == NULL)
		map = create_map(env_name, env_value);
	else
	{
		while (map->next != NULL)
			map = map->next;
		map->next = create_map(env_name, env_value);
		map->next->prev = map;
		while (map->prev != NULL)
			map = map->prev;
	}
	return (map);
}

char	*trim_env_name(char *env)
{
	char	*equal_pos;
	char	*name;

	if (env == NULL)
		return (NULL);
	equal_pos = ft_strchr(env, EQUAL_SIGN);
	if (equal_pos == NULL)
		return (NULL);
	name = (char *)ft_calloc(equal_pos - env + 1, sizeof(char));
	if (name == NULL)
		fatal_error("Malloc Error");
	ft_strlcpy(name, env, equal_pos - env + 1);
   
	return (name);
}

char	*trim_env_value(char *env)
{
	char	*equal_pos;
	char	*value;

	if (env == NULL)
		return (NULL);
	equal_pos = ft_strchr(env, EQUAL_SIGN);
	if (equal_pos == NULL)
		return (NULL);
	value = (char *)ft_calloc(ft_strlen(env) - (equal_pos - env + 1) + 1,
			sizeof(char));
	if (value == NULL)
		fatal_error("Malloc Error");
	ft_strlcpy(value, equal_pos + 1, ft_strlen(env) - (equal_pos - env + 1)
		+ 1);
	return (value);
}
// void	unset_env(char *env_name, t_var *map)
// {
//     if (map == NULL)
//         return;
    
//     t_var *next = map->next;  

//     if (ft_strncmp(map->name, env_name, ft_strlen(map->name)) == 0)
//     {
//         if (map->prev != NULL && map->next != NULL)
//         {
//             map->prev->next = map->next;
//             map->next->prev = map->prev;
//         }
//         else if (map->prev != NULL)
//             map->prev->next = NULL;
//         else if (map->next != NULL)
//             map->next->prev = NULL;  
//             printf("name? %p\n",map->value);
//         free(map->name);
//         free(map->value);
//         free(map);
//         unset_env(env_name, next);  
//         return;  
//     }
    
//     unset_env(env_name, next);  
// }

void unset_env(char *env_name, t_var *map)
{
    t_var *current = map;
    t_var *next;

    while (current != NULL) {
        next = current->next; // 次の要素を保持

        if (ft_strncmp(current->name, env_name, ft_strlen(current->name)) == 0)
        {
            if (current->prev != NULL)
                current->prev->next = current->next;
            if (current->next != NULL)
                current->next->prev = current->prev;

            free(current->name);
            free(current->value);
            free(current);
            
            // 指定された環境変数名が見つかったら、リンクの更新が完了したのでループを抜ける
            break;
        }

        current = next; // 次の要素へ移動
    }
}
