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

int builtin_unset(char  **args, t_var *env_map)
{
    int i;
    i = 1;
    while(args[i]!=NULL)
    {
        while (env_map != NULL)
        {
            if (!is_identifier(args[i]))
            {
                g_status = GENERAL_ERRORS;
                printf("minishell: unset: '%s': not a valid identifier\n", args[i]);
                return 0;
            }
            if (ft_strncmp(args[i], env_map->name, ft_strlen(args[i])) == 0)
                unset_env(args[i], env_map);
            env_map = env_map->next;
        }
        i++;
    }
    return 0;
}