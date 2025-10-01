#include "minishell.h"
#include <sys/wait.h>

static int	execute_pipeline_command(t_command *cmd, t_shell *shell)
{
	char	**argv;
	int		exit_status;

	if (cmd->subshell)
		return (execute_command_tree(cmd->subshell, shell));
	argv = get_argv_from_args(cmd);
	if (!argv || !argv[0])
	{
		if (argv)
			free_argv(argv);
		return (EXIT_SUCCESS);
	}
	setup_signals_default();
	if (is_builtin(argv[0]))
		exit_status = execute_builtin(argv, shell);
	else
		exit_status = execute_external(argv, shell);
	free_argv(argv);
	return (exit_status);
}

static void	setup_heredoc_if_first(t_command *current, int is_first)
{
	if (!is_first)
		return ;
	if (!current->input_redir || !current->input_redir->is_heredoc)
		return ;
	if (current->input_redir->fd < 0)
		return ;
	if (!safe_dup2(current->input_redir->fd, STDIN_FILENO, "dup2 heredoc"))
		exit(EXIT_FAILURE);
	close(current->input_redir->fd);
	current->input_redir->fd = -1;
}

void	exe_in_child(t_command *current, t_child_ctx *ctx, t_shell *shell)
{
	setup_signals_default();
	setup_heredoc_if_first(current, ctx->is_first);
	setup_pipeline_redir(current, ctx->prev_pipe_read, ctx->pipe_fd);
	exit(execute_pipeline_command(current, shell));
}
