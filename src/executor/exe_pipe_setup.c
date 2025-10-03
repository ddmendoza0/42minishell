/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipe_setup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:46:47 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:46:49 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*
 * PIPELINE FUNCTIONS - COMPLETAMENTE CORREGIDO
 */
int	safe_dup2(int oldfd, int newfd, const char *msg)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror(msg);
		return (0);
	}
	return (1);
}

void	setup_pipe_input(int prev_pipe_read)
{
	if (prev_pipe_read != -1)
	{
		if (!safe_dup2(prev_pipe_read, STDIN_FILENO, "dup2 stdin"))
			exit(EXIT_FAILURE);
		close(prev_pipe_read);
	}
}

void	setup_pipe_output(int *pipe_fd)
{
	close(pipe_fd[0]);
	if (!safe_dup2(pipe_fd[1], STDOUT_FILENO, "dup2 stdout"))
	{
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
}

void	setup_input_file(t_redir_file *input_redir)
{
	int	fd;

	if (!input_redir || input_redir->is_heredoc)
		return ;
	fd = open(input_redir->expanded_path, O_RDONLY);
	if (fd == -1)
	{
		perror(input_redir->expanded_path);
		exit(EXIT_FAILURE);
	}
	if (!safe_dup2(fd, STDIN_FILENO, "dup2 input"))
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

int	get_output_flags(t_redir_file *output_redir)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (output_redir->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	return (flags);
}
