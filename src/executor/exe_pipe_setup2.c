/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipe_setup2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:47:11 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 18:04:12 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*static void	setup_output_file(t_redir_file *output_redir)
{
	int	fd;
	int	flags;

	if (!output_redir)
		return ;
	flags = get_output_flags(output_redir);
	fd = open(output_redir->expanded_path, flags, 0644);
	if (fd == -1)
	{
		perror(output_redir->expanded_path);
		exit(EXIT_FAILURE);
	}
	if (!safe_dup2(fd, STDOUT_FILENO, "dup2 output"))
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	setup_pipeline_redir(t_command *cmd, int p_pipe_read, int *pipe_fd)
{
	setup_pipe_input(p_pipe_read);
	if (cmd->next && cmd->logic == CMD_PIPE)
		setup_pipe_output(pipe_fd);
	setup_input_file(cmd->input_redir);
	setup_output_file(cmd->output_redir);
}*/

static void	setup_output_file(t_redir_file *output_redir)
{
	int	fd;
	int	flags;

	if (!output_redir)
		return ;
	flags = get_output_flags(output_redir);
	fd = open(output_redir->expanded_path, flags, 0644);
	if (fd == -1)
	{
		perror(output_redir->expanded_path);
		exit(EXIT_FAILURE);
	}
	if (!safe_dup2(fd, STDOUT_FILENO, "dup2 output"))
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

void	setup_pipeline_redir(t_command *cmd, int p_pipe_read, int *pipe_fd)
{
	t_redir_file	*input;
	t_redir_file	*output;

	setup_pipe_input(p_pipe_read);
	if (cmd->next && cmd->logic == CMD_PIPE)
		setup_pipe_output(pipe_fd);
	input = get_last_redir(cmd->input_redirs);
	output = get_last_redir(cmd->output_redirs);
	setup_input_file(input);
	setup_output_file(output);
}
