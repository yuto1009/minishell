/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 15:22:40 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/02 17:56:38 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Readlineの出力先をstderrに変更 
Macに標準装備のReadlineはデフォルトで出力先がstderrになっているが、
GNUのreadlineがはstdoutになっているため、変更が必要。
rl_outstreamはGNU Readlineライブラリ内の変数 */
void set_output_destination(FILE *dst)
{
    rl_outstream = dst;
}

// 実装できないシステムコール関係のエラー
// ex: malloc, free
void fatal_error(char *message)
{
    ft_putendl_fd(message, STDERR_FILENO);
    exit(EXIT_FAILURE);
}

// bashでは実装されているが、minishellにおいて実装不要な部分で起こるエラーの時に呼び出す。
void minishell_error(char *message)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putendl_fd(message, STDERR_FILENO);
    // write(1,"\n",1);
    // exit(MINISHELL_ERROR);
}

// bashと同様の部分でエラーがある場合に呼び出す。
// exit_statusもbashと同様の値を返す。
void cmd_error_exit(char *location, char *message, int exit_status)
{
    const char *location_message = ft_strjoin(location, ": ");
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd((char *)location_message, STDERR_FILENO);
    ft_putendl_fd(message, STDERR_FILENO);
    free((char *)location_message);
    exit(exit_status);
}

int syntax_error_exit(char *token_str)
{
    const char *join_str = ft_strjoin(token_str,"'");
    ft_putstr_fd("minishell: ", STDERR_FILENO);    
    ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
    ft_putstr_fd((char *)join_str, STDERR_FILENO);
    write(1,"\n",1);
    free((char *)join_str);
    return 258;
    // exit(SYNTAX_ERROR);
}

int unsupported_token_msg(char *str)
{
    const char *join_str = ft_strjoin("sorry this command is unsupported : ", str);
    ft_putstr_fd((char *)join_str,STDERR_FILENO);
    write(1,"\n",1);
    return -1;
}