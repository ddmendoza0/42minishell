/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reviewer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:28:18 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 13:15:01 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_command_args(t_command *cmd, t_shell *shell)
{
	t_arg_token	*current;

	if (!cmd || !shell)
		return (0);
	current = cmd->args;
	while (current)
	{
		if (!current->expanded_value && current->original_token)
		{
			current->expanded_value
				= expand_token(current->original_token, shell);
			if (!current->expanded_value)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

static int	expand_command_redirections(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return (0);
	if (cmd->input_redir && !cmd->input_redir->expanded_path
		&& cmd->input_redir->original_token)
	{
		cmd->input_redir->expanded_path
			= expand_token(cmd->input_redir->original_token, shell);
		if (!cmd->input_redir->expanded_path)
			return (0);
	}
	if (cmd->output_redir && !cmd->output_redir->expanded_path
		&& cmd->output_redir->original_token)
	{
		cmd->output_redir->expanded_path
			= expand_token(cmd->output_redir->original_token, shell);
		if (!cmd->output_redir->expanded_path)
			return (0);
	}
	return (1);
}

static int	validate_command(t_command *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (!cmd->args && !cmd->input_redir && !cmd->output_redir && !cmd->subshell)
		return (1);
	if (cmd->input_redir && cmd->input_redir->expanded_path)
	{
		if (ft_strlen(cmd->input_redir->expanded_path) == 0)
			return (handle_error(shell, ERR_REDIRECT, "ambiguous redirect"));
	}
	if (cmd->output_redir && cmd->output_redir->expanded_path)
	{
		if (ft_strlen(cmd->output_redir->expanded_path) == 0)
			return (handle_error(shell, ERR_REDIRECT, "ambiguous redirect"));
	}
	return (1);
}

static int	process_single_command(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return (0);
	if (cmd->subshell)
	{
		if (!lexical_review(cmd->subshell, shell))
			return (0);
	}
	if (!expand_command_args(cmd, shell))
		return (0);
	if (!expand_command_redirections(cmd, shell))
		return (0);
	if (!validate_command(cmd, shell))
		return (0);
	return (1);
}

int	lexical_review(t_command *cmd_list, t_shell *shell)
{
	t_command	*current;

	if (!shell)
		return (0);
	current = cmd_list;
	while (current)
	{
		if (!process_single_command(current, shell))
			return (0);
		current = current->next;
	}
	return (1);
}
