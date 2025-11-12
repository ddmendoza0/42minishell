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
/*
int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;
	int	overflow;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
		cleanup_shell(shell);
	rl_clear_history();
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

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;
	int	overflow;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	
	// Cierra los FDs temporales de redirección si existen
	if (shell->temp_stdin >= 0)
		close(shell->temp_stdin);
	if (shell->temp_stdout >= 0)
		close(shell->temp_stdout);
	
	// Limpia los recursos del shell
	cleanup_shell(shell);
	rl_clear_history();
	
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
}*/

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;
	int	overflow;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	
	// PRIMERO verificar si hay demasiados argumentos
	// En este caso NO debemos salir
	if (argv[1] && argv[2])
	{
		print_error("minishell: exit", NULL, "too many arguments");
		return (EXIT_FAILURE);  // Retorna 1, NO sale del programa
	}
	
	// Solo si vamos a salir realmente, entonces limpiamos
	if (shell->temp_stdin >= 0)
		close(shell->temp_stdin);
	if (shell->temp_stdout >= 0)
		close(shell->temp_stdout);
	
	cleanup_shell(shell);
	rl_clear_history();
	
	// Si no hay argumentos, salir con el último exit status
	if (!argv[1])
		exit(shell->last_exit_status);
	
	// Verificar si es numérico
	if (!is_numeric_arg(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	
	// Parsear el código de salida
	exit_code = parse_exit_code(argv[1], &overflow);
	if (overflow)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	
	exit(exit_code);
}
