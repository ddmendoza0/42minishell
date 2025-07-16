#include "minishell.h"

static int	resize_argv(t_command *cmd, char *value)
{
    int i;
    int j;
    char **new_argv;

    i = 0;
    while (cmd->argv[i])
        i++;
    new_argv = malloc(sizeof(char *) * (i + 2));
    if (!new_argv)
        return 0;
    j = 0;
    while (j < i)
    {
        new_argv[j] = cmd->argv[j];
        j++;
    }
    new_argv[i] = ft_strdup(value);
    new_argv[i + 1] = NULL;
    free(cmd->argv);
    cmd->argv = new_argv;
    return 1;
}

int add_to_argv(t_command *cmd, char *value)
{
    if (!cmd || !value)
        return 0;
    if (!cmd->argv)
    {
        cmd->argv = malloc(sizeof(char *) * 2);
        if (!cmd->argv)
            return 0;
        cmd->argv[0] = ft_strdup(value);
        cmd->argv[1] = NULL;
    }
    else
    {
        if (!resize_argv(cmd, value))
            return 0;
    }
    return 1;
}