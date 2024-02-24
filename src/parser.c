/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:29:10 by kyoshida          #+#    #+#             */
/*   Updated: 2024/02/24 15:02:57 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node* createCommandNode(t_token* startToken,int index) {
    t_node* newNode = (t_node*)malloc(sizeof(t_node));
    newNode->token = startToken;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->currentin_fd = STDIN_FILENO;
    newNode->currentout_fd = STDOUT_FILENO;
    newNode->redirout_fd = 1;
    newNode->redirin_fd = 0;
    newNode->index = index;
    return newNode;
}

// コマンドとしてトークンを切り出すための関数
t_token* cutCommandTokens(t_token** current, t_token** nextCommandStart) {
    if (!*current) return NULL;

    t_token* start = *current;
    t_token* end = start;


    while (end->next!=NULL && end->next->kind!=TK_EOF && end->kind !=TK_OPERATOR) {
        end = end->next;
    }
    // printf("end : %s\n",end->str);

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

t_node* parser(t_token* tokens) {
    t_node* head = NULL;
    t_node* tail = NULL;
    int index = 1;
    t_token* currentToken = tokens;
    t_token* nextCommandStart = NULL;

    while (currentToken != NULL) {
        t_token* commandStart = cutCommandTokens(&currentToken, &nextCommandStart);
        
        t_node* newCommand = createCommandNode(commandStart,index);
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
