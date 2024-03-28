/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 14:29:17 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/28 16:24:50 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"


int builtin_pwd(t_var *env_map)
{
    t_var *tmp;
    tmp = env_map;
    while(tmp)
    {
        if(ft_strncmp(tmp->name,"PWD",3) == 0)
            printf("%s\n",tmp->value);
        tmp = tmp->next;
    }
    return 0;
}
