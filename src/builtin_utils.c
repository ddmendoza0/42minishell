
#include "minishell.h"

/*
 * ENVIRONMENT VARIABLE SETTER
 * This function updates or adds a variable in shell->env
 */
int set_env_var(t_shell* shell, char* var_assignment)
{
    char* var_name;
    char* equals_pos;
    int var_name_len;
    int i;
    int env_count;
    char** new_env;

    if (!shell || !var_assignment)
        return 0;

    equals_pos = ft_strchr(var_assignment, '=');
    if (!equals_pos)
        return 0;

    var_name_len = equals_pos - var_assignment;
    var_name = ft_strndup(var_assignment, var_name_len);
    if (!var_name)
        return 0;

    // Look for existing variable
    i = 0;
    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], var_name, var_name_len) == 0 &&
            shell->env[i][var_name_len] == '=')
        {
            // Replace existing variable
            free(shell->env[i]);
            shell->env[i] = ft_strdup(var_assignment);
            free(var_name);
            return (shell->env[i] != NULL);
        }
        i++;
    }

    // Add new variable
    env_count = i;
    new_env = malloc(sizeof(char*) * (env_count + 2));
    if (!new_env)
    {
        free(var_name);
        return 0;
    }

    // Copy existing environment
    i = 0;
    while (i < env_count)
    {
        new_env[i] = shell->env[i];
        i++;
    }

    // Add new variable
    new_env[env_count] = ft_strdup(var_assignment);
    new_env[env_count + 1] = NULL;

    if (!new_env[env_count])
    {
        free(new_env);
        free(var_name);
        return 0;
    }

    // Replace old environment
    free(shell->env);
    shell->env = new_env;
    free(var_name);

    return 1;
}