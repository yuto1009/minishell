#include "../include/minishell.h"

void builtin_unset(t_token *token,t_var *env_map)
{
    if (token->kind == TK_EOF)
        return ;

    while (*envmap != NULL)
    {
        if (ft_strncmp(token->str, *env_map, ft_strlen(token->str)) == 0)
            
    }
}