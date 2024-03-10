/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/10 16:02:33 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>

void printCommands(t_node* node) {
    while (node != NULL) {
        t_token* token = node->token;
        printf("nodeindex %d :",node->index);
        while (token != NULL && token->kind!=TK_EOF) {
            printf("%s ", token->str);
            token = token->next;
        }
        printf("\n");
        node = node->next;
    }
}

int interpret(char *line)
{
    struct s_node *node;
    int status;
    pid_t pid;
    t_token *token;
    token = tokenize(line);
    node = NULL ;
    expand(token);
    status = tokenize_error(token);
    if(status == 258 || status == 127)
    {
        free(token);
        return status;
    }
    
    node = parser(token);
    // printCommands(node);
    pid = exec(node);
    status = wait_pid(pid);
    return (status); // 仮
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }


bool is_only_blank_character(char *line)
{
    bool ans;
    ans = false;
    if(strcmp(line,"")==0)
        return true;
    while(is_blank(*line) && *line!='\0')
    {
        ans = true;
        line++;
    }
    return ans;
}

int main(void)
{
    char *line;
    int status;
    set_output_destination(stderr);
    status = 0;

    while(1)
    {
    setup_signal();
        line = readline("minishell$ ");
        if (line == NULL){
            // exit(1);
            break;
        }
        if(is_only_blank_character(line))
            continue;
             // breakをreturn (0)に変えるとリークが確認できる (テスターがNG出すようになる)
        if(*line)
            add_history(line); 
        status = interpret(line);
        free(line);
    }
    printf("exit\n"); // Ctrl+D ^Dが表示される

    return (status);
}