/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 23:03:00 by yutoendo          #+#    #+#             */
/*   Updated: 2024/03/23 12:54:27 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

char  *delete_pwd_elm(char *pwd)
{
    int i;
    int last_slash_ptr;
    char *ans;
    i = 0;
    while(pwd[i]!='\0')
    {
        if(pwd[i] == '/')
            last_slash_ptr = i;
        i++;
    }
    ans = (char *)ft_calloc(sizeof(char),last_slash_ptr+1);
    ft_strlcpy(ans , pwd , last_slash_ptr+1);
    free(pwd);
    return ans;
    
}
char *append_pwd(char *pwd , char **path)
{
    int i;
    char *src;
    char *ans;
    src = *path;
    i = 0;
    while((*path)[i] != '\0' && (*path)[i] !='/')
        i++;
    // printf(".pwd: %c\n",pwd[ft_strlen(pwd) - 1]);
    if(pwd[ft_strlen(pwd) - 1]!='/')
    ft_strlcat(pwd,"/",ft_strlen(pwd)+2);
    ans = (char *)malloc(sizeof(char)*i+1);
    ft_strlcpy(ans,src,i+1);   
    *path+=i;
    return  ft_strjoin(pwd , ans);;
}

char *new_pwd(char *prev_pwd, char *path)
{
    char *new_pwd;
    
    new_pwd = (char *)malloc(sizeof(char)*PATH_MAX);
    if(!prev_pwd)
        return (NULL);
    if(*path == '/')
        ft_strlcpy(new_pwd,"/",PATH_MAX);
    else
        ft_strlcpy(new_pwd,prev_pwd,PATH_MAX);
    while(*path)
    {
        if(*path == '/')
            path++;
        else if(ft_strncmp(path,"..",2) == 0)
        {
            new_pwd = delete_pwd_elm(new_pwd);
            path+=2;
        }
        else if(ft_strncmp(path,".",1) == 0)
             path++;
        else
            new_pwd = append_pwd(new_pwd,&path); 
    }
    return new_pwd;
}


int builtin_cd(char **args, t_var *env_map)
{
    char *home;
    char *path;
    char *prev_pwd;
    prev_pwd = get_env_value("PWD",env_map);
    unset_env("OLDPWD",env_map);
    export_env(env_map,"OLDPWD",prev_pwd);
    if(args[1] == NULL)
    {
        home = get_env_value("HOME",env_map);
        path = (char *)malloc(sizeof(char )* ft_strlen(home)+1);
        if(!home)
        {
            minishell_error("HOME not set");
            return (1);
        }
        ft_strlcpy(path,home,ft_strlen(home)+1);
    }
    else
    {
        path = (char *)malloc(sizeof(char )* ft_strlen(args[1])+1);
        ft_strlcpy(path , args[1],ft_strlen(args[1])+1);
    }
    if(chdir(path) < 0)
    {
       cd_error(path);
        return (1);
    }
    unset_env("PWD",env_map);
    export_env(env_map,"PWD",new_pwd(prev_pwd,path));
    return (0);
}























// void mini_cd(t_token **token)   // t_tokenの形で入力がある想定
// {
//     char *path = (*token)->next->str;   // 1つ目の引数はcdなので、2つ目の引数を取得する
    
//     if (path == NULL || *path == '\0')  // 引数がない場合は、ホームディレクトリに移動する
//     {
//         path = getenv("HOME");
//         if (path == NULL)
//         {
//             printf("cd: HOME not set\n");
//             return;
//         }
//     }
//     if (chdir(path) == -1)
//     {
//         printf("cd: %s: %s\n", path, strerror(errno));
//         return;
//     }
//     return;
// }

// // minishellに接続しないで動作確認するためのコード 
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// void mini_cd(const char *path)   // テスト用に文字列リテラルを想定
// {
//     if (path == NULL || *path == '\0')
//     {
//         path = getenv("HOME");
//         if (path == NULL)
//         {
//             printf("cd: HOME not set\n");
//             return;
//         }
//     }
//     if (chdir(path) == -1)
//     {
//         printf("cd: %s: %s\n", path, strerror(errno));
//         return;
//     }
//     return;
// }

// int main(void)
// {
//     // プロセスが終了するとディレクトリ変更の影響がなくなるため、
//     // 挙動をテストするには、
//     // system("pwd")で現在のディレクトリを表示させる。
//     mini_cd("..");
//     system("pwd");
//     mini_cd("");
//     system("pwd");
//     mini_cd("Desktop/");
//     system("pwd");
//     return (0);
// }