/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutoendo <yutoendo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 22:09:07 by yutoendo          #+#    #+#             */
/*   Updated: 2023/10/31 13:17:25 by yutoendo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include <readline/readline.h> // readline, add_history
#include <stdlib.h> // free exit getenv
#include <unistd.h> // fork execve access 
#include <stdbool.h> // boolean
#include <sys/wait.h> // wait

# define MINISHELL_ERROR 255

// error.c
void set_output_destination(FILE *dst);
void fatal_error(char *message);
void minishell_error(char *message);
void error_exit(char *location, char *message, int exit_status);
#endif