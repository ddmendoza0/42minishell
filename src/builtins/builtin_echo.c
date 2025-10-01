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

static int	check_n_flag(char *arg)
{
	int	j;

	if (!arg || arg[0] != '-' || !arg[1])
		return (0);
	j = 1;
	while (arg[j] == 'n')
		j++;
	if (arg[j] == '\0')
		return (1);
	return (0);
}

int	builtin_echo(char **argv, t_shell *shell)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && check_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		if (ft_putstr_fd(argv[i], STDOUT_FILENO) < 0)
			return (handle_system_error(shell, "write"));
		if (argv[i + 1])
		{
			if (ft_putstr_fd(" ", STDOUT_FILENO) < 0)
				return (handle_system_error(shell, "write"));
		}
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (set_exit_status(shell, EXIT_SUCCESS));
}
