/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reviewer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:28:18 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/03 18:01:18 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_command_redirections(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return (0);
	if (cmd->input_redirs && !expand_redir_list(cmd->input_redirs, shell))
		return (0);
	if (cmd->output_redirs && !expand_redir_list(cmd->output_redirs, shell))
		return (0);
	return (1);
}

static int	validate_redir_paths(t_redir_list *input, t_redir_list *output,
		t_shell *shell)
{
	t_redir_list	*current;

	current = input;
	while (current)
	{
		if (current->redir->expanded_path
			&& ft_strlen(current->redir->expanded_path) == 0)
			return (handle_error(shell, ERR_REDIRECT, "ambiguous redirect"));
		current = current->next;
	}
	current = output;
	while (current)
	{
		if (current->redir->expanded_path
			&& ft_strlen(current->redir->expanded_path) == 0)
			return (handle_error(shell, ERR_REDIRECT, "ambiguous redirect"));
		current = current->next;
	}
	return (1);
}

static int	validate_command(t_command *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (!cmd->args && !cmd->input_redirs && !cmd->output_redirs
		&& !cmd->subshell)
		return (1);
	if (!validate_redir_paths(cmd->input_redirs, cmd->output_redirs, shell))
		return (0);
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
