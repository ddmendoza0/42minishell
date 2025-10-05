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

static int	is_numeric(char *str)
{
	int	i;

	if (!str)
		return (0);
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

static int	get_exit_code(char *arg)
{
	long	result;

	result = ft_atoi(arg);
	return ((int)(result % 256));
}

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_code;

	if (!shell)
		return (EXIT_FAILURE);
	printf("exit\n");
	if (!argv[1])
	{
		exit_code = shell->last_exit_status;
		cleanup_shell(shell);
		exit(exit_code);
	}
	if (!is_numeric(argv[1]))
	{
		exit_error(shell, argv[1]);
		cleanup_shell(shell);
		exit(EXIT_MISUSE);
	}
	if (argv[2])
	{
		print_error("minishell: exit", NULL, "too many arguments");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	exit_code = get_exit_code(argv[1]);
	cleanup_shell(shell);
	exit(exit_code);
}
