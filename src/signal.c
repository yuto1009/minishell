/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshidakazushi <yoshidakazushi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 16:25:38 by yoshidakazu       #+#    #+#             */
/*   Updated: 2024/02/23 19:38:20 by yoshidakazu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sigint_handler(int sig_num) {
    // SIGINTを受け取ったときの処理
    // 例えば、プロンプトを再表示するなど
    write(STDOUT_FILENO, "\nminishell> ", 12);
    // 注意: シグナルハンドラ内では非同期シグナルセーフな関数のみを使用する
}

void set_signal()
{
    signal(SIGINT, sigint_handler);
}