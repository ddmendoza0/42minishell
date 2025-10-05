/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipe_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:46:20 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:46:20 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*
 * PIPELINE PARENT PROCESS
 */
void	handle_parent_fds(int *prev_pipe_read, t_command *current,
		int *pipe_fd)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (current->next && current->logic == CMD_PIPE)
	{
		close(pipe_fd[1]);
		*prev_pipe_read = pipe_fd[0];
	}
	else
		*prev_pipe_read = -1;
}

static int	get_process_exit_status(int status)
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
	return (0);
}

int	wait_pipeline(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	exit_status;

	exit_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			exit_status = get_process_exit_status(status);
		i++;
	}
	return (exit_status);
}

static void	close_pipe_fds(int prev_read, int *pipe_fd)
{
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (prev_read != -1)
		close(prev_read);
}

void	cleanup_pipeline_error(t_pipe_ctx *ctx, int *pipe_fd)
{
	int	i;

	close_pipe_fds(ctx->prev_pipe_read, pipe_fd);
	i = 0;
	while (i < ctx->index)
	{
		if (ctx->pids[i] > 0)
			waitpid(ctx->pids[i], NULL, 0);
		i++;
	}
	free(ctx->pids);
}
