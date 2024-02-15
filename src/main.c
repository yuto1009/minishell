/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/02/15 15:30:29 by kyoshida         ###   ########.fr       */
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

int heredoc(char *delimiter)
{
    char	*line;
	int		pfd[2];

        pipe(pfd);
    while (1)
    {
        line = readline("> ");
        if (line == NULL)
            break ;
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        
        char *temp = line; // lineの値を保持
        while(*temp != '\0') // tempを使って文字列を操作
        {
            write(pfd[1], temp, 1);
            temp++;
        }
        write(pfd[1], "\n", 1);

        free(line); // ここで安全にlineをfreeできる
    }
    
	close(pfd[1]);
    //    char buffer[1024];
    // int nbytes;

    // while ((nbytes = read(pfd[0], buffer, sizeof(buffer))) > 0) {
    //     write(STDOUT_FILENO, buffer, nbytes);
    // }
    return pfd[0];
}

bool is_redirection_in(t_token *token)
{
    // while(token->kind !=TK_EOF)
    // {
        if(token->kind == TK_REDIRECTION && ft_strncmp(token->str,"<",1) ==0)
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

    filename = node->token->next->str;
    if(ft_strncmp(node->token->str , ">>",2) == 0)
        node->redirout_fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
    else if(ft_strncmp(node->token->str , "<<",2) == 0)
        node->redirin_fd  = heredoc(filename);
    else if(ft_strncmp(node->token->str , ">",1) == 0 )
        node->redirout_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if(ft_strncmp(node->token->str , "<",1) == 0)
    {
        node->redirin_fd = open(filename,O_RDONLY);   
        if(node->redirin_fd == -1)
            printf("no such file or directory\n");
            // todo(ここにdup？）)
    }
    // if(node->redir_fd == -1)
        // todo("ファイルが存在しません");
    
        
    // return node;
}
int	stashfd(int fd)
{
	int	stashfd;
//fcntlは使用不可

	stashfd = fcntl(fd, F_DUPFD, 10);
	// if (stashfd < 0)
	// 	fatal_error("fcntl");
	// if (close(fd) < 0)
	// 	fatal_error("close");
	return (stashfd);
}


void redirect(t_node *node, char **token2argv)
{
	int fileoutfd, stashedout_targetfd = 0,stashedin_targetfd,fileinfd;
    extern char **environ;
	// 1. Redirect先のfdをopenする
	// fileoutfd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    fileoutfd = node->redirout_fd;
	fileoutfd = stashfd(fileoutfd); // filefdを退避させる
    fileinfd = node->redirin_fd;
    // printf("fileinfd : %d\n fileoutfd : %d\n",fileinfd,fileoutfd);
    
	// 2. Redirectする
    stashedin_targetfd = stashfd(node->currentin_fd); // targetfdを退避させる
	stashedout_targetfd = stashfd(node->currentout_fd); // targetfdを退避させる
	if (fileoutfd != node->currentout_fd)
	{
		dup2(fileoutfd, node->currentout_fd); // filefdをtargetfdに複製する（元々のtargetfdは閉じられる）
		close(fileoutfd);
	}
    if(fileinfd != node->currentin_fd)
    {
        dup2(fileinfd, node->currentin_fd);
        close(fileinfd);
    }
	// 3. コマンドを実行する
	execute(token2argv);
	// 4. Redirectしていたのを元に戻す
	dup2(stashedout_targetfd, node->currentout_fd); // 退避していたstashed_targetfdをtargetfdに複製する（元々のtargetfd）
    dup2(stashedin_targetfd, node->currentin_fd);
    
}

t_node *get_next_node(t_node *node)
{   
    // printf("DEBUG\n");
    if (node == NULL) return NULL;

    if (node->prev == NULL) return NULL;

    // if(node->prev->prev == NULL) return NULL;

    // 右の子がいれば、その最も左の子を探す && 今いるnodeが右の子じゃない
    if (node->prev->right != NULL && node != node->prev->right)
    {
        node = node->prev->right;
        while (node->left != NULL)
            node = node->left;
        return node;
    }
    // 右の子がいなければ、親を辿って適切なノードを探す
    if (node->prev != NULL && node == node->prev->right){
        node = node->prev;
        return node;
    }
    return node->prev;
}


void exec(t_node *node)
{
    char **token2argv;
    int len ,i =0 ;
    len = count_token_len(node->token);
    token2argv = (char **)ft_calloc(len+1,sizeof(char *));
    //node がまだうまく登れていない
    while(node!=NULL)
    {
        // if(node->token->kind!= TK_OPERATOR && fork()==0)
        while(node->token->kind !=TK_EOF)
        {
            if(node->token->kind == TK_REDIRECTION)
            {
                open_file(node);
                node->token = node->token->next;
            }
            else
            {
                token2argv[i] = node->token->str;
                i++;
            }
            node->token = node->token->next;
        }
        node->currentout_fd = 1;
        node->currentin_fd = 0;
        redirect(node ,token2argv);
        node = get_next_node(node);
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
    // TEST_PRINT_NODE(node); 
    return (0);
    
    // char **argv = token_to_argv(token);
    //ここからとりあえずbuiltinを実装 comment by kyoshidaz
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