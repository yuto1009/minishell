/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/03/23 17:10:56 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node* create_command_node(t_token* startToken,int index) 
{
    t_node* newNode = (t_node*)malloc(sizeof(t_node));
    if (!newNode) {
        fatal_error("Malloc");
    }
    newNode->token = startToken;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->currentin_fd = 0;
    newNode->currentout_fd = 1;
    newNode->redirout_fd = 1;
    newNode->redirin_fd = 0;
    newNode->pipe_in[0] = STDIN_FILENO;
    newNode->pipe_out[1] = STDOUT_FILENO;
    newNode->pipe_out[0] = -1;
    newNode->pipe_in[1] = -1;  
    newNode->index = index;
    return newNode;
}

// コマンドとしてトークンを切り出すための関数
static t_token* cut_command_tokens(t_token** current, t_token** nextCommandStart) 
{
    if (!*current) 
        return NULL;
    t_token* start = *current;
    t_token* end = start;
    while (end->next!=NULL && end->next->kind!=TK_EOF && end->kind !=TK_OPERATOR) 
    {
        end = end->next;
    }
    if(end!=NULL&&end->kind==TK_OPERATOR)
    {
        *nextCommandStart = end->next;
        end->kind =TK_EOF;
        end = NULL;
    }
    else
    *nextCommandStart =NULL;
    return start;
}

t_node* parser(t_token* tokens) 
{
    t_node* head = NULL;
    t_node* tail = NULL;
    int index = 1;
    t_token* currentToken = tokens;
    t_token* nextCommandStart = NULL;

    while (currentToken != NULL) 
    {
        t_token* commandStart = cut_command_tokens(&currentToken, &nextCommandStart);
        
        t_node* newCommand = create_command_node(commandStart,index);
        if (head == NULL) {
            head = newCommand;
        } else {
            tail->next = newCommand;
            newCommand->prev = tail;
        }
        tail = newCommand;
        index++;
        currentToken = nextCommandStart;
    }
    return head;
}
