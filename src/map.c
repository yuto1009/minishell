/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuendo <yuendo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:56:14 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/16 18:23:31 by yuendo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void export_env(t_var *map, char *env_name, char *env_value);


static char *trim_env_name(char *env)
{
    char * equal_pos;
    char *name;
    
    if (env == NULL)
        return NULL;
    equal_pos = ft_strchr(env, EQUAL_SIGN);
    if (equal_pos == NULL)
        printf("Equal sign not found\n");   // 多分このif文いらない
    const size_t name_len = equal_pos - env + 1;
    name = (char *)ft_calloc(name_len, sizeof(char));
    if (name == NULL)
        fatal_error("Malloc Error");
    ft_strlcpy(name, env, name_len);
    
    return (name);
}


static char *trim_env_value(char *env)
{
    char *equal_pos;
    char *value;

    if (env == NULL)
        return NULL;
    equal_pos = (env, EQUAL_SIGN);
    if (equal_pos == NULL)
        printf("Equal sign not found\n");   // 多分このif文いらない
    const size_t value_len = ft_strlen(env) - (equal_pos - env + 1) + 1;
    value = (char *)ft_calloc(1, sizeof(char));
    if (value == NULL)
        fatal_error("Malloc Error");
    ft_strlcpy(value, equal_pos+1, value_len);

    return value;
}

// init_env_map
// グローバル変数env_mapを初期化する
t_var * init_env_map(void)
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
        environ++;
    }
    return (env_map);
}

char *get_env_value(char *env_name, t_var *map)
{
    size_t name_len;
    name_len = ft_strlen(env_name);
    
    while(map!=NULL)
    {
        if(ft_strncmp(map->name, env_name, name_len) == 0)
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
        ans = ft_strjoin(map->name, "=");
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



void unset_env(char *env_name,t_var *map)
{
    // t_var 
    // while(map!=NULL)
    // {
    //     if(map->name == env_name)
            
    //     map = map->next;
    // }
    if (map == NULL)
        return;
    if (ft_strncmp(map->name, env_name, ft_strlen(map->name)) == 0)
    {
        if (map->prev != NULL && map->next != NULL)
        {
            map->prev->next = map->next;
            map->next->prev = map->prev;
        }
        else if (map->prev != NULL)
            map->prev->next == NULL
        else
            map->next->prev == NULL;
        free(map); 
    }
    return (unset_env(env_name, map->next));
}

static t_var *create_map(char *name , char *value)
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
    if(map == NULL)
        map = create_map(env_name,env_value);
    else
    {
        while(map->next!=NULL)
            map = map->next;
        
        map->next = create_map(env_name,env_value);
        map->next->prev = map;    
    }
    
} 


    
