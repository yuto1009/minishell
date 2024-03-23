/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:45:42 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/20 13:13:54 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

int builtin_env(t_var *env_map)
{
    // error処理まだ
    
  while(env_map != NULL)
  {
    printf("%s=%s\n", env_map->name, env_map->value);
    env_map = env_map->next;
  }
  return 0;
}

