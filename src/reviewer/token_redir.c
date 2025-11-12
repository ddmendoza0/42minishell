/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:54:38 by dmendoza          #+#    #+#             */
/*   Updated: 2025/11/12 16:10:00 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_redir_file_token(t_token *file_token, t_token **token,
				t_shell *shell)
{
	if (!file_token)
	{
		*token = NULL;
		return (handle_syntax_error(shell, "newline"));
	}
	if (file_token->type != WORD)
	{
		*token = NULL;
		return (handle_syntax_error(shell, file_token->value));
	}
	return (1);
}

static int	create_and_add_input_redir(t_command *cmd, t_token *file_token,
				int is_heredoc)
{
	t_redir_file	*new_redir;

	new_redir = create_redir_file(file_token, 0, is_heredoc);
	if (!new_redir)
		return (0);
	new_redir->order = get_next_redir_order(cmd);
	add_redir_to_list(&cmd->input_redirs, new_redir);
	return (1);
}

int	add_token_redir_in(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token			*redir_token;
	t_token			*file_token;
	int				is_heredoc;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!validate_redir_file_token(file_token, token, shell))
		return (0);
	is_heredoc = (redir_token->type == HEREDOC);
	if (!create_and_add_input_redir(cmd, file_token, is_heredoc))
		return (0);
	*token = file_token->next;
	return (1);
}

static int	create_and_add_output_redir(t_command *cmd, t_token *file_token,
				int append_mode)
{
	t_redir_file	*new_redir;

	new_redir = create_redir_file(file_token, append_mode, 0);
	if (!new_redir)
		return (0);
	new_redir->order = get_next_redir_order(cmd);
	add_redir_to_list(&cmd->output_redirs, new_redir);
	return (1);
}

int	add_token_redir_out(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token			*redir_token;
	t_token			*file_token;
	int				append_mode;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!validate_redir_file_token(file_token, token, shell))
		return (0);
	append_mode = (redir_token->type == APPEND_OUT);
	if (!create_and_add_output_redir(cmd, file_token, append_mode))
		return (0);
	*token = file_token->next;
	return (1);
}
