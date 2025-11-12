/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:30:07 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/11/12 15:40:00 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_fd_values(t_shell *shell)
{
	shell->last_exit_status = 0;
	shell->temp_stdin = -1;
	shell->temp_stdout = -1;
	shell->history_fd = -1;
}

static int	setup_pwd_variable(t_shell *shell, char *current_dir)
{
	char	*pwd_str;

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
	free(pwd_str);
	return (1);
}

static int	duplicate_std_fds(t_shell *shell)
{
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

static int	init_pwd_and_fds(t_shell *shell, char *current_dir)
{
	init_fd_values(shell);
	if (!setup_pwd_variable(shell, current_dir))
		return (0);
	if (!duplicate_std_fds(shell))
		return (0);
	return (1);
}

int	init_shell(t_shell *shell, char **envp)
{
	char	*current_dir;

	if (!copy_environment(shell, envp))
	{
		perror("minishell: malloc");
		return (0);
	}
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
	{
		perror("minishell: getcwd");
		cleanup_shell(shell);
		return (0);
	}
	shell->cwd = current_dir;
	if (!init_pwd_and_fds(shell, current_dir))
		return (0);
	return (1);
}
