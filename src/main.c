/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/17 20:02:23 by kyoshida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
// int g_status =0 ;
void printCommands(t_node* node)
{
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

void interpret(char *line, t_var *env_map)
{
    struct s_node *node;
    int status;
    int prev_status;
    pid_t pid;
    t_token *token;
    
    prev_status = g_status;
    g_status = 0;
    token = tokenize(line);
    node = NULL ;
    expand(token,env_map,prev_status);    
    if(g_status == MINISHELL_ERROR)
        return ;
    status = tokenize_error(token);
    if(status == 258 || status == 127)
    {
        free(token);
        g_status = status;
        return ;
    }
    node = parser(token);
    // printCommands(node);
    pid = exec(node);
    wait_pid(pid);
    return ; // 仮
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

void roop_readline(void)
{
    // int status = 0;
    t_var *env_map;
    char *line;
    int exit_status;
    env_map = init_env_map();
    // while(env_map != NULL){
    //     printf("name : %s ; value : %s;\n", env_map->name ,env_map->value);
    //     env_map = env_map->next;
    // }
    set_output_destination(stderr);
        g_status = 0;
    while(1)
    {
        setup_signal();
        line = readline("minishell$ ");
        exit_status  = 0;
        if (line == NULL){
            break;
        }
        if(is_only_blank_character(line))
            continue;
             // breakをreturn (0)に変えるとリークが確認できる (テスターがNG出すようになる)
        if(*line)
            add_history(line); 
        interpret(line, env_map);
        free(line);
    }
    return ;
}

int main(void)
{
    int status;
    set_output_destination(stderr);
    status = 0;
    roop_readline();
    printf("exit\n"); // Ctrl+D ^Dが表示される
    return (g_status);
}