/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/02/07 17:59:37 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include <stdio.h> // DEBUG

bool is_path_executable(const char *path)
{
    int result = access(path, X_OK);
    if (result < 0)
    {
        return (false);
    }
    return (true);
}

char *trim_single_path(char *paths)
{
    size_t i;
    char *path;

    i = 0;
    while(paths[i] != '\0')
    {
        if (paths[i] == ':')
            break;
        i++;
    }
    path = (char *)malloc(sizeof(char)*(i) + 2);
    if (path == NULL)
    {
        fatal_error("Malloc Error");
    }
    ft_strlcpy(path, paths, i+1);
    ft_strlcat(path, "/", i+2);
    return path;
}


char *search_path(char *filename)
{
    extern char **environ;
    char *paths = getenv("PATH");
    char *path;
    char *executable;
    
    path = NULL;
    executable = NULL;
    while(paths != NULL)
    {
        path = trim_single_path(paths);
        paths = ft_strchr(paths, ':');
        if (paths != NULL)
        {
            paths++;
        }
        executable = ft_strjoin(path, filename);
        if (executable == NULL)
        {
            fatal_error("Malloc Error");
        }
        if (access(executable, F_OK) == 0)
        {
            free(path);
            return (executable);
        }
        else
        {
            free(executable);
            free(path);
        }
    }
    return (NULL);
}

int execute(char **argv)
{
    int wstatus;
    extern char **environ;
    char *executable;
    pid_t pid = fork();
    
    if (pid < 0)
        fatal_error("fork");
    if (pid == 0)
    {
        // // 子プロセス
        if (ft_strchr(argv[0], '/') == NULL)
        {
            executable = search_path(argv[0]);
        }
        else
        {
            executable = argv[0];
        }
        execve(executable, argv, environ);
        cmd_error_exit(executable, "command not found", 127);
    }
    else 
    {
        // 親プロセス
        wait(&wstatus);
        return (WEXITSTATUS(wstatus));
    }
    return (WEXITSTATUS(wstatus));
}

void TEST_print_token(t_token *token)
{
    while (token->kind != TK_EOF)
    {
        printf("%s : ", token->str);
        if (token->kind == TK_WORD)
            printf("WORD\n");
        else if (token->kind == TK_OPERATOR)
            printf("OPERATOR\n");
        else if (token->kind == TK_REDIRECTION)
            printf("REDIRECTION\n");
        else 
            printf("PRESERVED\n");
        token = token->next;
    }
}
void TEST_PRINT_NODE(t_node *node) {
    if (node == NULL) {
        return; // Handle NULL pointer
    }

    // Check for EOF token and print if it's not EOF
    if (node->token->kind != TK_EOF && node->left != NULL && node->right != NULL) {
        printf("str: %s\n", node->token->str); // Added newline for readability
        printf("right: %s\n", node->right->token->str);
        printf("left: %s\n", node->left->token->str);
    }
    if(node->redirection)
    printf("prev :%s \n",node->redirection->token->str);
    // Recursive calls with NULL check
    TEST_PRINT_NODE(node->left);
    TEST_PRINT_NODE(node->right);
}

int count_token_len(t_token *token)
{
    int i;

    i = 0;
    while(token!= NULL)
    {
        i++;
        token = token->next;
    }
    return (i-1);
}

void tokenize_error(t_token *token)
{
    if(token == NULL)
        return ;
    if((token->kind == TK_OPERATOR || token->kind == TK_REDIRECTION)&& count_token_len(token) == 1)
        syntax_error_exit("newline");
    while(token->kind!= TK_EOF && token->next->kind!=TK_EOF)
    {
        if(token->kind == TK_OPERATOR && token->next->kind == TK_OPERATOR)
        {
            syntax_error_exit(token->next->str);            
        }
        else if(token->kind == TK_REDIRECTION && token->next->kind == TK_REDIRECTION)
        {
            syntax_error_exit(token->next->str);
        }
        token = token->next;
    }
    if(token->kind == TK_REDIRECTION&&token->next->kind == TK_EOF)
        syntax_error_exit("newline");
}

bool is_redirection_out(t_token *token)
{
    // while(token->kind !=TK_EOF)
    // {
        if(token->kind == TK_REDIRECTION && ft_strncmp(token->str,">",1) ==0)
            return true;
        // token = token->next;
    // }
    return false;
        // printf("left token : %s\n right token %s\n",node->token->str);
    
}
t_node *start_node(t_node*node)
{
    while(node->left!=NULL)
        node = node->left;
    return node;
}

void open_file(t_node *node)
{
    char *filename;
    // int filefd;
    while(node->token->kind!=TK_REDIRECTION)
        node->token = node->token->next;
    filename = node->token->next->str;
    node->redir_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    // if(node->redir_fd == -1)
        // todo("ファイルが存在しません");
    
        
    // return node;
}



void exec(t_node *node)
{
    char **token2argv;
    int len ,i =0 ;
    len = count_token_len(node->token);
    token2argv = (char **)ft_calloc(len+1,sizeof(char *));
    while(node!=NULL)
    {
        while(node->token->kind !=TK_EOF)
        {
            if(is_redirection_out(node->token))
            {
                open_file(node);
                // printf("token ; %s\n",node->token->str);
                node->token = node->token->next;
            }
            else{
            token2argv[i] = node->token->str;
            printf("argv : %s\n",token2argv[i]);
            i++;
                
            }
            node->token = node->token->next;
        }
                // printf("%d\n",node->redir_fd);
        dup2(node->redir_fd,1);
        execute(token2argv);
            node = node->prev;
    }
}

int interpret(char *line)
{
    struct s_node *node = (struct s_node *)malloc(sizeof(struct s_node));      
    t_token *token = tokenize(line);
    tokenize_error(token);
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    // TEST_print_token(token);   
    
    // node = parser(node);
    node = parser(token);
    node = start_node(node);
    exec(node);
    // printf("prev : %s\n",node->left->prev->token->str);
    // exec_cmd(node);
    // todo("redireciton_do");
    TEST_PRINT_NODE(node); 
        return (0);
    
    // char **argv = token_to_argv(token);
    //ここからとりあえずbuiltinを実装 comment by kyoshida
    // if(ft_strncmp(argv[0], "exit",4) == 0)
    //   return mini_exit(argv);
    // else if(ft_strncmp(argv[0],"env",3) == 0)
    //   return mini_env(argv);
    //ここまで
    
    // int i = 0;
    // while(argv[i])
    // {
    //     printf("argv: %s\n", argv[i]);
    //     i++;
    // }
    // int status = execute(argv);
    // printf("status: %d\n", status);
    // return (status);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }

int main(void)
{
    char *line;
    int status;

    set_output_destination(stderr);
    status = 0;
    while(1)
    {
        line = readline("minishell$ ");
        if (line == NULL)
            break;  // breakをreturn (0)に変えるとリークが確認できる (テスターがNG出すようになる)
        add_history(line); 
        status = interpret(line);
        free(line);
    }
    return (status);
}