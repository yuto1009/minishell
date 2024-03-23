/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/23 00:09:21 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
// int g_status =0 ;
void printCommands(t_node* node)
{
    while (node != NULL) {
        t_token* token = node->token;
        // printf("nodeindex %d :",node->index);
        while (token != NULL && token->kind!=TK_EOF) {
            printf("token : %s \n", token->str);
            token = token->next;
        }
        printf("\n");
        node = node->next;
    }
}
int count_token_len(t_token *token)
{
    int i;
    t_token *tmp;
    tmp = token;
    i = 0;
    while(tmp!= NULL)
    {
        i++;
        tmp = tmp->next;
    }
    free(tmp);
    return (i-1);
}
void interpret(char *line, t_var *env_map)
{
    struct s_node *node;
    int status;
    int prev_status;
    char **token2argv;
    pid_t pid;
    t_token *token;
    
    prev_status = g_status;
    g_status = 0;
    token = tokenize(line);
    node = NULL ;
    if(expand(token,env_map,prev_status)<0)
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

        if(node->next == NULL&&is_buildin(node->token->str)){
            token2argv= serch_redir(node,count_token_len(node->token));
                if(!token2argv)
                    exit(1);
            dup_fd(node);
           g_status =  exec_buildin(token2argv,env_map,prev_status);
            reset_fd(node);
        }
        else
        {
            pid = exec(node, env_map,prev_status);
            wait_pid(pid);
        }
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
    if(*line != '\0')
        ans = false;
    return ans;
}

void roop_readline(void)
{
    // int status = 0;
    t_var *env_map;
    char *line;
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
    set_output_destination(stderr);
    roop_readline();
    printf("exit\n"); // Ctrl+D ^Dが表示される
    return (g_status);
}