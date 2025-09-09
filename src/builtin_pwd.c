/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:26:51 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:26:53 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **argv, t_shell *shell)
{
	char	*cwd;

	if (!shell)
		return (pwd_error(shell));
	if (argv[1])
	{
		print_error("minishell: pwd", NULL, "too many arguments");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (shell->cwd)
		{
			printf("%s\n", shell->cwd);
			return (set_exit_status(shell, EXIT_SUCCESS));
		}
		else
			return (pwd_error(shell));
	}
	printf("%s\n", cwd);
	if (!shell->cwd || ft_strncmp(shell->cwd, cwd, ft_strlen(cwd) + 1) != 0)
	{
		if (shell->cwd)
			free(shell->cwd);
		shell->cwd = ft_strdup(cwd);
	}
	free(cwd);
	return (set_exit_status(shell, EXIT_SUCCESS));
}
