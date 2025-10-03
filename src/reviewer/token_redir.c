/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:54:38 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 18:45:25 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static int	get_heredoc_mode(t_token *redir_token)
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

static int	get_heredoc_mode(t_token *redir_token)
{
	if (redir_token->type == HEREDOC)
		return (1);
	return (0);
}

int	add_token_redir_in(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token			*redir_token;
	t_token			*file_token;
	int				is_heredoc;
	t_redir_file	*new_redir;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	is_heredoc = get_heredoc_mode(redir_token);
	new_redir = create_redir_file(file_token, 0, is_heredoc);
	if (!new_redir)
		return (0);
	add_redir_to_list(&cmd->input_redirs, new_redir);
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
	t_token			*redir_token;
	t_token			*file_token;
	int				append_mode;
	t_redir_file	*new_redir;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	append_mode = get_append_mode(redir_token);
	new_redir = create_redir_file(file_token, append_mode, 0);
	if (!new_redir)
		return (0);
	add_redir_to_list(&cmd->output_redirs, new_redir);
	*token = file_token->next;
	return (1);
}*/

static int	get_next_redir_order(t_command *cmd)
{
	t_redir_list	*in;
	t_redir_list	*out;
	int				max_order;

	max_order = 0;
	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->order > max_order)
			max_order = in->redir->order;
		in = in->next;
	}
	out = cmd->output_redirs;
	while (out)
	{
		if (out->redir->order > max_order)
			max_order = out->redir->order;
		out = out->next;
	}
	return (max_order + 1);
}

int	add_token_redir_in(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token			*redir_token;
	t_token			*file_token;
	int				is_heredoc;
	t_redir_file	*new_redir;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	is_heredoc = (redir_token->type == HEREDOC);
	new_redir = create_redir_file(file_token, 0, is_heredoc);
	if (!new_redir)
		return (0);
	new_redir->order = get_next_redir_order(cmd);
	add_redir_to_list(&cmd->input_redirs, new_redir);
	*token = file_token->next;
	return (1);
}

int	add_token_redir_out(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token			*redir_token;
	t_token			*file_token;
	int				append_mode;
	t_redir_file	*new_redir;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	append_mode = (redir_token->type == APPEND_OUT);
	new_redir = create_redir_file(file_token, append_mode, 0);
	if (!new_redir)
		return (0);
	new_redir->order = get_next_redir_order(cmd);
	add_redir_to_list(&cmd->output_redirs, new_redir);
	*token = file_token->next;
	return (1);
}
