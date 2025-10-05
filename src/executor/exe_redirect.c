/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:48:06 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 19:46:18 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static int	setup_input_redirection(t_redir_file *input_redir, t_shell *shell)
{
	int	fd;

	if (!input_redir)
		return (1);
	if (input_redir->is_heredoc && input_redir->fd >= 0)
		fd = input_redir->fd;
	else
		fd = open(input_redir->expanded_path, O_RDONLY);
	if (fd == -1)
	{
		handle_system_error(shell, input_redir->expanded_path);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		handle_system_error(shell, "dup2");
		return (0);
	}
	close(fd);
	if (input_redir->is_heredoc)
		input_redir->fd = -1;
	return (1);
}

static int	setup_output_redirection(t_redir_file *output_redir, t_shell *shell)
{
	int	fd;
	int	flags;

	if (!output_redir)
		return (1);
	flags = O_WRONLY | O_CREAT;
	if (output_redir->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(output_redir->expanded_path, flags, 0644);
	if (fd == -1)
	{
		handle_system_error(shell, output_redir->expanded_path);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		handle_system_error(shell, "dup2");
		return (0);
	}
	close(fd);
	return (1);
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		if (dup2(saved_stdin, STDIN_FILENO) == -1)
			perror("restore stdin");
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		if (dup2(saved_stdout, STDOUT_FILENO) == -1)
			perror("restore stdout");
		close(saved_stdout);
	}
}

int	save_standard_fds(int *s_stdin, int *s_stdout, t_shell *shell)
{
	*s_stdin = dup(STDIN_FILENO);
	*s_stdout = dup(STDOUT_FILENO);
	if (*s_stdin == -1 || *s_stdout == -1)
	{
		if (*s_stdin != -1)
			close(*s_stdin);
		if (*s_stdout != -1)
			close(*s_stdout);
		handle_system_error(shell, "dup");
		return (0);
	}
	return (1);
}

int	app_redir(t_command *cmd, int s_stdin, int s_stdout, t_shell *shell)
{
	t_redir_file	*input;
	t_redir_file	*output;

	input = get_last_redir(cmd->input_redirs);
	output = get_last_redir(cmd->output_redirs);
	if (input)
	{
		if (!setup_input_redirection(input, shell))
		{
			restore_redirections(s_stdin, s_stdout);
			return (0);
		}
	}
	if (output)
	{
		if (!setup_output_redirection(output, shell))
		{
			restore_redirections(s_stdin, s_stdout);
			return (0);
		}
	}
	return (1);
}
