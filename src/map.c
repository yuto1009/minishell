/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:56:14 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/16 16:48:03 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void export_env(t_var *map, char *env_name, char *env_value);



static char *trim_env_value(char *env)
{
    
}

static char *trim_env_name(char *env)
{
    
}

// init_env_map
// グローバル変数env_mapを初期化する
void init_env_map(void)
{
    extern char **environ;
    char *name;
    char *value;
    t_var *env_map;

    env_map = (t_var *)ft_calloc(1, sizeof(t_var));
    if (env_map == NULL)
        fatal_error("Malloc Error");

    while (*environ != NULL)
    {
        name = trim_env_name(*environ);
        value = trim_env_value(*environ);
        export_env(env_map, name, value);
    }
}

char *get_env_value(char *env_name, t_var *map)
{
    int name_len;
    name_len = ft_strlen(env_name);
    
    while(map!=NULL)
    {
        if(ft_strncmp(map->name,env_name,name_len) == 0)
            return map->value;      
        map = map->next;
    }
    return NULL;
}

char *get_env_list(t_var *map)
{
    char *ans;
    char *tmp;
    while(map!=NULL)
    {
        ans = ft_strjoin(map->name , "=");
        tmp = ft_strjoin(ans , map->value);
        free(ans);
        ans = tmp;
        free(tmp);
        tmp = ft_strjoin(ans, "\n");
        free(ans);
        ans = tmp;
        free(tmp);
        map = map->next;
    }
    return ans;
}

void unset_env(char *env_name)
{
    
}

t_var *create_map(char *name , char *value)
{
    t_var *map;
    map = (t_var *)ft_calloc(1,sizeof(t_var));
    if(map == NULL)
        fatal_error("malloc error");
    map->name = name;
    map->value = value;
    map->prev = NULL;
    map->next = NULL; 
    return (map);
}

void export_env(t_var *map, char *env_name, char *env_value)
{
    t_var *head;
    head = map;
    while(map!=NULL)
        map = map->next;
    
    map->next = create_map(env_name,env_value);
    map->next->prev = map;
    
} 


    
