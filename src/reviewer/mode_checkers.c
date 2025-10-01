#include "minishell.h"

int	is_heredoc_mode(t_command *cmd)
{
	if (!cmd || !cmd->input_redir)
		return (0);
	return (cmd->input_redir->is_heredoc);
}
