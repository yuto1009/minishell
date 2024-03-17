#include "../include/minishell.h"

static bool is_identifier(char *name)
{
	if (name == NULL || *name == '\0')
		return (false);
	if (!ft_isalpha(*name) && *name != '_')
		return (false);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

void builtin_unset(t_token *token,t_var *env_map)
{
    if (token->kind == TK_EOF)
        return ;
    token = token->next;
    while (env_map != NULL)
    {
        if (is_identifier(token->str) != true)
		{
			g_status = GENERAL_ERRORS;
			printf("minishell: unset: '%s': not a valid identifier\n", token->str);
			return ;
		}
        if (ft_strncmp(token->str, env_map->name, ft_strlen(token->str)) == 0)
            unset_env(token->str, env_map);
        env_map = env_map->next;
    }
    return (builtin_unset(token->next,env_map));
}