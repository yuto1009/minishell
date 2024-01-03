/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 17:56:12 by yutoendo          #+#    #+#             */
/*   Updated: 2024/01/03 22:45:12 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/built_in.h"

// 動作確認してないぜ
void mini_echo(t_token **token) 
{
    t_token *current;
    bool print_newline;
    
    if (token == NULL || *token == NULL)
     {
        printf("\n");
        return;
    }
    current = *token;
    print_newline = true;
    if (ft_strncmp(current->str, OP_N, ft_strlen(OP_N)) == 0) 
    {
        current = current->next;
        print_newline = false;
    }

    while (current != NULL) 
    {
        printf("%s", current->str);
        current = current->next;
        if (current != NULL) 
        {
            printf(" ");
        }
    }

    if (print_newline == true) 
    {
        printf("\n");
    }
}