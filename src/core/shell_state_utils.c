/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:39:05 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/11/12 16:39:15 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_environment(t_shell *shell)
{
	int	i;

	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
}

void	close_file_descriptors(t_shell *shell)
{
	if (shell->stdin_backup >= 0)
	{
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	if (shell->stdout_backup >= 0)
	{
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
	if (shell->history_fd >= 0)
	{
		close(shell->history_fd);
		shell->history_fd = -1;
	}
}

void	cleanup_shell(t_shell *shell)
{
	free_environment(shell);
	if (shell->cwd)
		free(shell->cwd);
	close_file_descriptors(shell);
}

int	count_env_vars_s(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

int	copy_environment(t_shell *shell, char **envp)
{
	int	env_count;
	int	i;

	env_count = count_env_vars_s(envp);
	shell->env = malloc(sizeof(char *) * (env_count + 1));
	if (!shell->env)
		return (0);
	i = 0;
	while (i < env_count)
	{
		shell->env[i] = ft_strdup(envp[i]);
		if (!shell->env[i])
		{
			while (--i >= 0)
				free(shell->env[i]);
			free(shell->env);
			shell->env = NULL;
			return (0);
		}
		i++;
	}
	shell->env[i] = NULL;
	return (1);
}
