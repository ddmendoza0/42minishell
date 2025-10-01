#include "minishell.h"

static int	get_heredoc_mode(t_token *redir_token)
{
	if (redir_token->type == HEREDOC)
		return (1);
	return (0);
}

int	add_token_redir_in(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token	*redir_token;
	t_token	*file_token;
	int		is_heredoc;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	is_heredoc = get_heredoc_mode(redir_token);
	if (cmd->input_redir)
		free_redir_file(cmd->input_redir);
	cmd->input_redir = create_redir_file(file_token, 0, is_heredoc);
	if (!cmd->input_redir)
		return (0);
	*token = file_token->next;
	return (1);
}

static int	get_append_mode(t_token *redir_token)
{
	if (redir_token->type == APPEND_OUT)
		return (1);
	return (0);
}

int	add_token_redir_out(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token	*redir_token;
	t_token	*file_token;
	int		append_mode;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	append_mode = get_append_mode(redir_token);
	if (cmd->output_redir)
		free_redir_file(cmd->output_redir);
	cmd->output_redir = create_redir_file(file_token, append_mode, 0);
	if (!cmd->output_redir)
		return (0);
	*token = file_token->next;
	return (1);
}
