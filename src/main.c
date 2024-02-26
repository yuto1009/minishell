/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/02/25 23:05:06 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#include <stdio.h> // DEBUG

int siginit = 0;

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
    return;
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
    if(token->kind == TK_REDIRECTION && ft_strncmp(token->str,">",1) ==0)
        return true;
    return false;
    
}

int heredoc(char *delimiter)
{
    char	*line;
	int		pfd[2];

        pipe(pfd);
        siginit = 2;
    while (1)
    {
        line = readline("> ");
        
        if (line == NULL)
        {
            printf("\n");
            exit(1);
            // break ;
        }
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

    return pfd[0];
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
            // printf("no such file or directory\n");
            // todo(ここにdup？）)
    }
        if(node->redirin_fd == -1)
            exit(1);
    // if(node->redir_fd == -1)
        // todo("ファイルが存在しません");
    
        
    // return node;
}
int	stashfd(int fd)
{
	int	stashfd;
    //fcntlは使用不可

	stashfd = fcntl(fd, F_DUPFD, 10);
	return (stashfd);
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




void redirect(t_node *node)
{
	int fileoutfd,fileinfd;
    // int stashedout_targetfd = 0,stashedin_targetfd;
    extern char **environ;
	// 1. Redirect先のfdをopenする
	// fileoutfd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    fileoutfd = node->redirout_fd;
    fileinfd = node->redirin_fd;
	// fileoutfd = stashfd(fileoutfd); // filefdを退避させる
    // printf("fileinfd : %d\n fileoutfd : %d\n",fileinfd,fileoutfd);
	// 2. Redirectする
    // stashedin_targetfd = stashfd(node->currentin_fd); // targetfdを退避させる
	// stashedout_targetfd = stashfd(node->currentout_fd); // targetfdを退避させる
	if (fileoutfd != 1)
	{
		dup2(fileoutfd, node->currentout_fd); // filefdをtargetfdに複製する（元々のtargetfdは閉じられる）
		close(fileoutfd);
	}
    if(fileinfd != 0)
    {
        dup2(fileinfd, node->currentin_fd);
        close(fileinfd);
    }
	// 3. コマンドを実行する
	// execute(token2argv);
	// 4. Redirectしていたのを元に戻す
	// dup2(stashedout_targetfd, node->currentout_fd); // 退避していたstashed_targetfdをtargetfdに複製する（元々のtargetfd）
    // dup2(stashedin_targetfd, node->currentin_fd);
    
}

// ここのロジックでノードを上に登る
t_node *get_next_node(t_node *node)
{   
    if (node == NULL) return NULL;

    if (node->prev == NULL) return NULL;

    // 右の子がいれば、その最も左の子を探す && 今いるnodeが右の子じゃない
    if (node->prev->right != NULL && node != node->prev->right)
    {
        node = node->prev->right;
        while (node->left != NULL)
            node = node->left;
        return node;
    }
    // 右の子がいなければ、親を辿って適切なノードを探す
    return node->prev;
}



void execute_pipe(char **argv,int output_fd,int input_fd)
{
    extern char **environ;
    char *executable;

        if(output_fd!=1){
        dup2(output_fd,1);
        close(output_fd);
        }
        if(input_fd!=0){
        dup2(input_fd,0);
        close(input_fd);
        }
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
        // exit(1);

}

void do_pipe(t_node *node,int outpfd , int end_index)
{
     if(node->index == 1){
                node->currentout_fd = outpfd;
            }
            else if(node->index == end_index){
                node->currentin_fd =node->prev->pipe_in;
            }
            else
            {
                node->currentout_fd = outpfd;
                node->currentin_fd =node->prev->pipe_in;                
            }
}

 int serch_endindex(t_node *node)
{
    t_node *tmp;
    int count = 0;
    tmp = node;
    while(tmp!=NULL){
        count++;
        tmp = tmp->next;
    }
    return count;
}

void child_handler(int sig)
{
    if(sig)
    ;
      rl_replace_line("", 0);
    printf("\n");
        rl_on_new_line();

}


int exec(t_node *node)
{
    char **token2argv;
    int len ,i =0 ;
        int pfd[2];
        // int exit_status;
        // int status;
    len = count_token_len(node->token);
    int end_index;
    // int status;
    end_index = serch_endindex(node);
        siginit = 1;
    
    while(node != NULL)
    { 
        if(end_index>1 && node->index!=end_index)
            pipe(pfd);
        if(fork() == 0){
            // signal(SIGINT,child_handler);
            token2argv = (char **)ft_calloc(len+1,sizeof(char *));
            i = 0;
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
            do_pipe(node,pfd[1],end_index);
            // printf("current_OUT : %d \ncurrent_IN: %d\n\n",node->currentout_fd,node->currentin_fd);
            redirect(node);
            // printf("argv : %s\n",token2argv[0]);
            execute_pipe(token2argv,node->currentout_fd,node->currentin_fd);
        }
        // if(node->index!=1)
        //     close(node->prev->pipe_in);
        // if(node->index!=end_index)
        //     close(pfd[1]);
            // printf("pfd[0] : %d\n",pfd[0]);
        // if(node->index!=1)
        node->pipe_in = pfd[0];
        // printf("out_fd :%d\n in_fd : %d \n",node->currentout_fd,node->currentin_fd);
        node = node->next;
    
    }
        // wait(&status);
        // exit_status = WEXITSTATUS(status);
        if(pfd[0]!=0)
        close(pfd[0]);
        if(pfd[1] != 1)
        close(pfd[1]);
    for(int i = 0 ; i<end_index;i++)
        wait(NULL);
    // return exit_status;
    return 0;
}

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
    struct s_node *node = NULL ;
    int status;
    t_token *token = tokenize(line);
    tokenize_error(token);
    if(node)
    ;
    node = parser(token);
 
    // printCommands(node);
    status = exec(node);
    // printf("exit_status : %d\n" , status);
    return (status); // 仮
    // int status = execute(argv);
    // return (status);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q minishell");
// }
void sig_handler(int sig_num) {
    // SIGINTを受け取ったときの処理
    // 例えば、プロンプトを再表示するなど
    if(sig_num)
    ;
    
    // write(STDOUT_FILENO, "minishell$\n", 12);
    if(sig_num == SIGINT){
    // siginit = true;
    if(siginit==0){
        rl_replace_line("", 0);
        printf("\n");
        rl_on_new_line();
        rl_redisplay();
    }
    else if(siginit == 1)
    {
        rl_replace_line("", 0);
        printf("\n");
        rl_on_new_line();
        
    }
    else if(siginit == 2)
    {
        rl_replace_line("", 0);
        rl_on_new_line();
        exit(1);
        rl_redisplay();
    }
    }
                
    if(sig_num == SIGQUIT)
    {
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
        printf("\n");
    }
    // 注意: シグナルハンドラ内では非同期シグナルセーフな関数のみを使用する
}

void set_signal()
{
    // struct sigaction sa;
    // sigemptyset(&sa.sa_mask);
    // sa.sa_handler = sig_handler;
    // sa.sa_flags = 0;
    // sigaction(SIGINT,&sa,NULL);
    // sigaction(SIGQUIT,&sa,NULL);
    signal(SIGINT, sig_handler);
    signal(SIGQUIT,sig_handler);
}


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
    set_signal();
    status = 0;

    while(1)
    {
        siginit = 0;
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