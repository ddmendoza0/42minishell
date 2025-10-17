/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reviewer_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:45:46 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/17 12:45:48 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_command_args(t_command *cmd, t_shell *shell)
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

int	expand_redir_list(t_redir_list *list, t_shell *shell)
{
	t_redir_list	*current;

	current = list;
	while (current)
	{
		if (!current->redir->expanded_path
			&& current->redir->original_token)
		{
			current->redir->expanded_path
				= expand_token(current->redir->original_token, shell);
			if (!current->redir->expanded_path)
				return (0);
		}
		current = current->next;
	}
	return (1);
}
