/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:30:07 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/03 19:51:03 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_shell(t_shell *shell)
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
	if (shell->cwd)
		free(shell->cwd);
	if (shell->stdin_backup >= 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup >= 0)
		close(shell->stdout_backup);
}

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static int	copy_environment(t_shell *shell, char **envp)
{
	int	env_count;
	int	i;

	env_count = count_env_vars(envp);
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

int	init_shell(t_shell* shell, char** envp)
{
	char* pwd_str;
	char* current_dir;

	// IMPORTANTE: Obtener el directorio actual PRIMERO
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		perror("minishell: getcwd");
		return (0);
	}

	// DEBUG - Eliminar después
	fprintf(stderr, "DEBUG getcwd: %s\n", current_dir);

	if (!copy_environment(shell, envp))
	{
		perror("minishell: malloc");
		free(current_dir);
		return (0);
	}

	shell->cwd = current_dir;

	pwd_str = ft_strjoin("PWD=", current_dir);
	if (!pwd_str)
	{
		cleanup_shell(shell);
		return (0);
	}

	if (!set_env_var(shell, pwd_str))
	{
		free(pwd_str);
		cleanup_shell(shell);
		return (0);
	}

	// DEBUG - Eliminar después
	fprintf(stderr, "DEBUG PWD env: %s\n", get_env_value(shell, "PWD"));

	free(pwd_str);

	shell->last_exit_status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	if (shell->stdin_backup == -1 || shell->stdout_backup == -1)
	{
		perror("minishell: dup");
		cleanup_shell(shell);
		return (0);
	}
	return (1);
}
