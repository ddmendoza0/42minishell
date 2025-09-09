/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:25:51 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:25:55 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
