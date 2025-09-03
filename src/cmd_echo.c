#include "minishell.h"

int builtin_echo(char **argv, t_shell *shell)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    while (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
    {
        newline = 0;
        i++;
    }
    while (argv[i])
    {
        ft_putstr_fd(argv[i], STDOUT_FILENO);
        if (argv[i + 1])
            ft_putstr_fd(" ", STDOUT_FILENO);
        i++;
    }
    if (newline)
        ft_putstr_fd("\n", STDOUT_FILENO);
    return set_exit_status(shell, EXIT_SUCCESS);
}