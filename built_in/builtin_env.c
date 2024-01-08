/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 18:45:42 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/01/08 19:44:25 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

t_var *create_kv(char *environ)
{
  char **kv_str;
  t_var *new;
  kv_str = ft_split(environ, '=');
  if(!kv_str)
    return NULL;
  new = (t_var *)ft_calloc(1, sizeof(t_var));
  new->key = kv_str[0];
  new->value = kv_str[1];
  free(kv_str);
  return new;
}


t_var *env_to_var(char **environ)
{
  int i ;
  i = 0;
  t_var *last;
  t_var *start;
  t_var *new;
  while(environ[i] != NULL)
  {
    new = create_kv(environ[i]);
    if(last!=NULL)
      last->next = new;
    last = new;
    if(start == NULL)
      start = new;
    // printf("%s\n", environ[i]);
    i++;
  }
  return start;
}

int mini_env(char **argv)
{
   extern char **environ; // 環境変数へのポインタ
   t_var *var;
    (void)argv;

    var = env_to_var(environ);
    while(var != NULL)
    {
      printf("%s=%s\n", var->key, var->value);
      var = var->next;
    }
    return 0;
}

// int main()
// {
//   mini_env(NULL);
//   return 0;
// }