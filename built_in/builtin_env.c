/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:45:42 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/17 20:27:55 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

// t_var *create_kv(char *environ)
// {
//   char **kv_str;
//   t_var *new;
//   kv_str = ft_split(environ, '=');
//   if(!kv_str)
//     return NULL;
//   new = (t_var *)ft_calloc(1, sizeof(t_var));
//   new->name = kv_str[0];
//   new->value = kv_str[1];
//   free(kv_str);
//   return new;
// }

// t_var *env_to_var(char **environ)
// {
//   int i ;
//   i = 0;
//   t_var *last;
//   t_var *start;
//   t_var *new;
//   while(environ[i] != NULL)
//   {
//     new = create_kv(environ[i]);
//     if(last!=NULL)
//       last->next = new;
//     last = new;
//     if(start == NULL)
//       start = new;
//     // printf("%s\n", environ[i]);
//     i++;
//   }
//   return start;
// }

void builtin_env(t_var *env_map)
{
  //  extern char **environ; // 環境変数へのポインタ
  //  t_var *var;
  //   (void)argv;

  while(env_map != NULL)
  {
    printf("%s=%s\n", env_map->name, env_map->value);
    env_map = env_map->next;
  }
  //   var = env_to_var(environ);
  //   while(var != NULL)
  //   {
  //     printf("%s=%s\n", var->key, var->value);
  //     var = var->next;
  //   }
  // printf("%s", get_env_list(env_map));
}

// int main()
// {
//   mini_env(NULL);
//   return 0;
// }