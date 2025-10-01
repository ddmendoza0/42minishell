/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:26:08 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:26:09 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv, t_shell *shell)
{
	int	i;

	if (!shell)
		return (set_exit_status(shell, EXIT_FAILURE));
	if (argv[1])
	{
		print_error("minishell: env", NULL, "too many arguments");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	if (!shell->env)
		return (set_exit_status(shell, EXIT_FAILURE));
	i = 0;
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	return (set_exit_status(shell, EXIT_SUCCESS));
}
