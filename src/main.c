/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 11:27:27 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/27 12:24:17 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int last_status;

void interpret(char *const line, int *status)
{   
    t_token *token;
    t_node *node;
    
    token = tokenize(line); // トークン化（字句解析）
    if (at_eof(token))
        ;
    else if (syntax_error)
        *status = ERROR_TOKENIZE;
    else
    {
        node = parse(token);    // 構文解析
        if (syntax_error)
        {
            *status = ERROR_PARSE;
        }
        else
        {
            expand(node);
            *status = exec(node);
        }
        free_node(node);
    }
    free_token(token);
}

int main(void)
{
    char *line;
    int status;
    
    rl_outstream = stderr;
    setup_signal();
    last_status = EXIT_SUCCESS;
    while (1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;
        if (*line)
            add_history(line);
        interpret(line, &last_status);
        free(line);
    }
    exit(last_status);
}
