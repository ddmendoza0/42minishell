#include "minishell.h"
#include <sys/wait.h>

int	init_pipe_ctx(t_pipe_ctx *ctx, t_command *cmd_lst, t_shell *sh)
{
	ctx->cmd_count = count_pipeline_commands(cmd_lst);
	ctx->pids = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->pids)
		return (handle_error(sh, ERR_MALLOC, "pipeline execution"));
	ctx->prev_pipe_read = -1;
	ctx->index = 0;
	return (0);
}

int	crt_pipe_if(t_command *crr, int *p_fd, t_pipe_ctx *ctx, t_shell *sh)
{
	if (crr->next && crr->logic == CMD_PIPE)
	{
		if (pipe(p_fd) == -1)
		{
			handle_system_error(sh, "pipe");
			cleanup_pipeline_error(ctx, NULL);
			return (0);
		}
	}
	return (1);
}

int	fork_exe(t_command *curr, int *p_fd, t_pipe_ctx *ctx, t_shell *sh)
{
	int			has_pipe;
	t_child_ctx	child_ctx;

	has_pipe = (curr->next && curr->logic == CMD_PIPE);
	ctx->pids[ctx->index] = fork();
	if (ctx->pids[ctx->index] == -1)
	{
		handle_system_error(sh, "fork");
		if (has_pipe)
			cleanup_pipeline_error(ctx, p_fd);
		else
			cleanup_pipeline_error(ctx, NULL);
		return (0);
	}
	else if (ctx->pids[ctx->index] == 0)
	{
		child_ctx.prev_pipe_read = ctx->prev_pipe_read;
		child_ctx.pipe_fd = p_fd;
		child_ctx.is_first = (ctx->index == 0);
		exe_in_child(curr, &child_ctx, sh);
	}
	else
		handle_parent_fds(&ctx->prev_pipe_read, curr, p_fd);
	return (1);
}
