/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyoshida <kyoshida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:08:35 by yutoendo          #+#    #+#             */
/*   Updated: 2024/02/26 17:00:50 by kyoshida         ###   ########.fr       */
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



int heredoc(char *delimiter)
{
    char    *line;
    int     pfd[2];

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
int stashfd(int fd)
{
    int stashfd;
    //fcntlは使用不可

    stashfd = fcntl(fd, F_DUPFD, 10);
    return (stashfd);
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
}

void execute_pipe(char **argv)
{
    extern char **environ;
    char *executable;

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

char **serch_redir(t_node *node,int len)
{
    int i;
    i = 0;
    char **token2argv;
    token2argv = (char **)ft_calloc(len+1,sizeof(char *));
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
    return token2argv;
}


int exec(t_node *node)
{
    char **token2argv;
    int len;
    int end_index;
    int pid;
    end_index = serch_endindex(node);
        siginit = 1;

    while(node != NULL)
    { 
        len = count_token_len(node->token);
        set_pipe(node,end_index);
        pid = fork();
        if(pid < 0)
            cmd_error_exit("fork","fork error",1);
        else if (pid == 0){
        token2argv= serch_redir(node,len);
            dup_child_pipe(node);
            redirect(node);
            execute_pipe(token2argv);
        }
        set_parent_pipe(node);
        node = node->next;
    }
    return pid;
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

int wait_pid(pid_t pid)
{
    	pid_t	wait_result;
	int		status;
	int		wstatus;

	while (1)
	{
		wait_result = wait(&wstatus);
		if (wait_result == pid)
			status = WEXITSTATUS(wstatus);
		else if (wait_result < 0)
		{
			if (errno == ECHILD)
				break ;
		}
	}
	return (status);
}

int interpret(char *line)
{

    struct s_node *node = NULL ;
    int status;
    pid_t pid;
    t_token *token = tokenize(line);
    tokenize_error(token);
    if(node)
    ;
    node = parser(token);
 
    // printCommands(node);
    pid = exec(node);
    status = wait_pid(pid);
    // printf("exit_status : %d\n" , status);
    return (status); // 仮

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
        rl_redisplay();
        rl_on_new_line();
        rl_replace_line("", 0);
        // printf("\n");
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