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

static int	check_overflow(long long num, int digit, int sign)
{
	if (sign == 1)
	{
		if (num > (LLONG_MAX - digit) / 10)
			return (1);
	}
	else
	{
		if (num > (LLONG_MAX - digit) / 10)
			return (1);
	}
	return (0);
}

static int	parse_exit_code(const char *str, int *overflow)
{
	long long	num;
	int			sign;
	int			i;

	num = 0;
	sign = 1;
	i = 0;
	*overflow = 0;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (check_overflow(num, str[i] - '0', sign))
		{
			*overflow = 1;
			return (2);
		}
		num = num * 10 + (str[i] - '0');
		i++;
	}
	num *= sign;
	return ((num % 256 + 256) % 256);
}

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;
	int	overflow;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!argv[1])
		exit(shell->last_exit_status);
	if (!is_numeric_arg(argv[1]))
	{
		exit_error(shell, argv[1]);
		exit(EXIT_MISUSE);
	}
	if (argv[2])
	{
		print_error("minishell: exit", NULL, "too many arguments");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	exit_code = parse_exit_code(argv[1], &overflow);
	if (overflow)
	{
		exit_error(shell, argv[1]);
		exit(EXIT_MISUSE);
	}
	exit(exit_code);
}
