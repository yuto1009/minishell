/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 12:08:13 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/03/02 17:48:54 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
int count_args(char **args)
{
    int i;

    i = 0;
    while (args[i] != NULL)
        i++;
    return (i);
}

static bool check_long_overflow(char *str,int sign)
{
  int i;
  i = 0;
  while(str[i])
  {
    if(i == 19)
    {
      if(sign == 1 && str[i] > '7')
        return false;
      else if(sign == -1 && str[i] > '8')
        return false;
    }
    else if(i > 19)
      return false;
    i++;
  }
  return true;
}
void print_numeric_error(char *str)
{
  printf("minishel: exit: %s: numeric argument required\n",str);
  // exit(255);
}
static void atol_exit(char *str)
{
    //256以上の場合終了コードは256で割ったあまりを返す
  // char *tmpstr;
  int i;
  int sign;
  int num;

  num = 0;
  i = 0;
  sign = 1;

  if(str[i] == '-')
  {
    sign = -1;
    i++;
  }
  if(!ft_isdigit(str[i]) || !check_long_overflow(str, sign))
  {
    print_numeric_error(str);
    // ft_putstr_fd("exit : numeric argument required\n",STDERR_FILENO);
    exit(255);
  }
  while(str[i])
    num = num*10 + (str[i++]-'0');
  if(num>256)
    num = num%256;
    // printf("exit_status : %d\n", num);
  exit(num);
}

int mini_exit(char **args)
{
    int arg_len;

    arg_len = 0;
    arg_len = count_args(args);
    printf("exit\n");
    if(arg_len>2)
    {
      minishell_error("exit : too many arguments");
      return (1);
    }
    else if(arg_len == 2)
      atol_exit(args[1]);
    else
      exit(0); // 最後のステータスを保持しておく必要あり
    return (0);
}