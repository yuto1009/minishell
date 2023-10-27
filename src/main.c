/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/27 23:22:38 by yutoendo         ###   ########.fr       */
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

int main(void)
{
    char *line;

    set_output_destination(stderr);
    while(1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        add_history(line);
        free(line);
    }
    return (0);
}