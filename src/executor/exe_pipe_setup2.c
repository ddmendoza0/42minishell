#include "minishell.h"
#include <sys/wait.h>

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

static void	setup_pipeline_redir(t_command *cmd, int p_pipe_read, int *pipe_fd)
{
	setup_pipe_input(p_pipe_read);
	if (cmd->next && cmd->logic == CMD_PIPE)
		setup_pipe_output(pipe_fd);
	setup_input_file(cmd->input_redir);
	setup_output_file(cmd->output_redir);
}
