/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:47:46 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:47:47 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

int	execute_pipeline(t_command *cmd_list, t_shell *shell)
{
	t_pipe_ctx		ctx;
	t_command		*current;
	int				pipe_fd[2];
	int				exit_status;

	if (init_pipe_ctx(&ctx, cmd_list, shell))
		return (EXIT_FAILURE);
	current = cmd_list;
	while (current && ctx.index < ctx.cmd_count)
	{
		if (!crt_pipe_if(current, pipe_fd, &ctx, shell))
			return (EXIT_FAILURE);
		if (!fork_exe(current, pipe_fd, &ctx, shell))
			return (EXIT_FAILURE);
		current = current->next;
		ctx.index++;
	}
	if (ctx.prev_pipe_read != -1)
		close(ctx.prev_pipe_read);
	exit_status = wait_pipeline(ctx.pids, ctx.cmd_count);
	free(ctx.pids);
	return (set_exit_status(shell, exit_status));
}
