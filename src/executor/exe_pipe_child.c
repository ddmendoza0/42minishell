/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipe_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:46:05 by dmendoza          #+#    #+#             */
/*   Updated: 2025/11/12 15:48:00 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static void	check_path_permissions(char *path, char *cmd, t_shell *shell)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			handle_error(shell, ERR_IS_DIRECTORY, cmd);
			free(path);
			exit(EXIT_CANNOT_EXECUTE);
		}
	}
	if (access(path, X_OK) != 0)
	{
		handle_error(shell, ERR_PERMISSION_DENIED, cmd);
		free(path);
		exit(EXIT_CANNOT_EXECUTE);
	}
}

static void	execute_external_direct(char **argv, t_shell *shell)
{
	char	*executable_path;

	executable_path = get_and_validate_path(argv, shell);
	check_path_permissions(executable_path, argv[0], shell);
	execve(executable_path, argv, shell->env);
	handle_system_error(shell, argv[0]);
	free(executable_path);
	exit(EXIT_CANNOT_EXECUTE);
}

static void	execute_pipeline_command(t_command *cmd, t_shell *shell)
{
	char	**argv;
	int		exit_status;

	if (cmd->subshell)
		exit(execute_command_tree(cmd->subshell, shell));
	argv = get_argv_from_args(cmd);
	if (!argv || !argv[0])
	{
		if (argv)
			free_argv(argv);
		exit(EXIT_SUCCESS);
	}
	setup_signals_default();
	if (is_builtin(argv[0]))
	{
		exit_status = execute_builtin(argv, cmd, shell);
		free_argv(argv);
		exit(exit_status);
	}
	else
		execute_external_direct(argv, shell);
}

static void	setup_heredoc_if_first(t_command *current, int is_first)
{
	t_redir_file	*input;

	if (!is_first)
		return ;
	input = get_last_redir(current->input_redirs);
	if (!input || !input->is_heredoc)
		return ;
	if (input->fd < 0)
		return ;
	if (!safe_dup2(input->fd, STDIN_FILENO, "dup2 heredoc"))
		exit(EXIT_FAILURE);
	close(input->fd);
	input->fd = -1;
}

void	exe_in_child(t_command *current, t_child_ctx *ctx, t_shell *shell)
{
	setup_signals_default();
	setup_heredoc_if_first(current, ctx->is_first);
	if (!validate_command_redirections(current, shell))
		exit(1);
	setup_pipeline_redir(current, ctx->prev_pipe_read, ctx->pipe_fd);
	execute_pipeline_command(current, shell);
}
