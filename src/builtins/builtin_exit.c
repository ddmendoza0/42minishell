/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:26:22 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:26:24 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric_arg(const char *str)
{
    int	i;

    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!str[i])
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

static int	parse_exit_code(const char *str)
{
    long long	num;
    int			sign;
    int			i;

    num = 0;
    sign = 1;
    i = 0;
    if (str[i] == '-')
        sign = -1;
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i])
    {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    num *= sign;
    return ((num % 256 + 256) % 256);
}

int	builtin_exit(char **argv, t_shell *shell)
{
    int	exit_code;

    ft_putstr_fd("exit\n", STDOUT_FILENO);
    if (!argv[1])
    {
        exit_code = shell->last_exit_status;
        exit(exit_code);
    }
    if (!is_numeric_arg(argv[1]))
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(argv[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        exit(2);
    }
    if (argv[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (set_exit_status(shell, 1));
    }
    exit_code = parse_exit_code(argv[1]);
    exit(exit_code);
}
