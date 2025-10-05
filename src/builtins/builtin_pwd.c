/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:26:51 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/03 19:45:11 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_getcwd_error(t_shell *shell)
{
	if (shell->cwd)
	{
		printf("%s\n", shell->cwd);
		return (set_exit_status(shell, EXIT_SUCCESS));
	}
	else
		return (pwd_error(shell));
}

static void	update_shell_cwd(t_shell *shell, char *cwd)
{
	if (!shell->cwd || ft_strncmp(shell->cwd, cwd, ft_strlen(cwd) + 1) != 0)
	{
		if (shell->cwd)
			free(shell->cwd);
		shell->cwd = ft_strdup(cwd);
	}
}

int	builtin_pwd(char **argv, t_shell *shell)
{
	char	*cwd;

	if (!shell)
		return (EXIT_FAILURE);
	(void)argv;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (handle_getcwd_error(shell));
	printf("%s\n", cwd);
	update_shell_cwd(shell, cwd);
	free(cwd);
	return (set_exit_status(shell, EXIT_SUCCESS));
}
