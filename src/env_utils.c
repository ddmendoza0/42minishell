#include "minishell.h"

// Get environment variable value
char* get_env_value(t_shell* shell, char* var_name)
{
    int i;
    int var_len;

    if (!shell || !shell->env || !var_name)
        return (NULL);

    var_len = ft_strlen(var_name);
    i = 0;
    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], var_name, var_len) == 0
            && shell->env[i][var_len] == '=')
        {
            return (&shell->env[i][var_len + 1]);
        }
        i++;
    }
    return (NULL);
}

// Handle special variables like $?, $, etc.
char* get_special_var(t_shell* shell, char* var_name)
{
    static char buffer[32];

    if (!shell || !var_name)
        return (NULL);

    if (ft_strncmp(var_name, "?", 2) == 0)
    {
        snprintf(buffer, sizeof(buffer), "%d", shell->last_exit_status);
        return (buffer);
    }
    else if (ft_strncmp(var_name, "$", 2) == 0)
    {
        snprintf(buffer, sizeof(buffer), "%d", getpid());
        return (buffer);
    }
    else if (ft_strncmp(var_name, "0", 2) == 0)
    {
        return ("minishell");
    }

    return (NULL);
}

// Main function to get any variable (regular or special)
char* expand_variable(t_shell* shell, char* var_name)
{
    char* value;

    if (!shell || !var_name)
        return (NULL);

    // Check special variables first
    value = get_special_var(shell, var_name);
    if (value)
        return (ft_strdup(value));

    // Check environment variables
    value = get_env_value(shell, var_name);
    if (value)
        return (ft_strdup(value));

    // Variable not found, return empty string
    return (ft_strdup(""));
}