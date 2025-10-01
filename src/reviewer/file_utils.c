#include "minishell.h"

static char	*get_input_path(t_command *cmd)
{
	if (cmd->input_redir->expanded_path)
		return (cmd->input_redir->expanded_path);
	return (cmd->input_redir->original_token->value);
}

char	*get_input_file(t_command *cmd)
{
	if (!cmd || !cmd->input_redir)
		return (NULL);
	return (ft_strdup(get_input_path(cmd)));
}

static char	*get_output_path(t_command *cmd)
{
	if (cmd->output_redir->expanded_path)
		return (cmd->output_redir->expanded_path);
	return (cmd->output_redir->original_token->value);
}

char	*get_output_file(t_command *cmd)
{
	if (!cmd || !cmd->output_redir)
		return (NULL);
	return (ft_strdup(get_output_path(cmd)));
}

int	is_append_mode(t_command *cmd)
{
	if (!cmd || !cmd->output_redir)
		return (0);
	return (cmd->output_redir->append_mode);
}
