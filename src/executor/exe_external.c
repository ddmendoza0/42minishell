/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_external.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:45:04 by dmendoza          #+#    #+#             */
/*   Updated: 2025/11/07 10:37:20 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static void	execute_child_process(char *exe_path, char **argv, t_shell *shell)
{
	setup_signals_default();
	execve(exe_path, argv, shell->env);
	handle_system_error(shell, argv[0]);
	exit(EXIT_CANNOT_EXECUTE);
}

static int	handle_child_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit: 3\n", 8);
		return (128 + sig);
	}
	return (EXIT_FAILURE);
}

static int	validate_executable(char *executable_path, char **argv,
		t_shell *shell)
{
	struct stat	path_stat;

	if (stat(executable_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			free(executable_path);
			return (handle_error(shell, ERR_IS_DIRECTORY, argv[0]));
		}
	}
	if (access(executable_path, X_OK) != 0)
	{
		free(executable_path);
		return (handle_error(shell, ERR_PERMISSION_DENIED, argv[0]));
	}
	return (0);
}

static int	fork_and_execute(char *executable_path, char **argv,
		t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		free(executable_path);
		return (handle_system_error(shell, "fork"));
	}
	else if (pid == 0)
		execute_child_process(executable_path, argv, shell);
	free(executable_path);
	waitpid(pid, &status, 0);
	return (handle_child_status(status));
}

/*
 * EXTERNAL COMMAND EXECUTION
 */
int	execute_external(char **argv, t_shell *shell)
{
	char	*executable_path;
	int		validation_result;

	executable_path = find_executable(argv[0], shell);
	if (!executable_path)
	{
		if (ft_strchr(argv[0], '/'))
			return (handle_error(shell, ERR_NO_SUCH_FILE, argv[0]));
		return (handle_error(shell, ERR_COMMAND_NOT_FOUND, argv[0]));
	}
	validation_result = validate_executable(executable_path, argv, shell);
	if (validation_result != 0)
		return (validation_result);
	return (fork_and_execute(executable_path, argv, shell));
}
