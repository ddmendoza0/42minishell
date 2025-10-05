/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:22:45 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 13:23:47 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_word_token(t_command *cmd, t_token **current)
{
	if (!add_token_to_args(cmd, *current))
		return (0);
	*current = (*current)->next;
	return (1);
}

int	handle_logic_token(t_command *cmd, t_token *current, t_command **new_cmd)
{
	if (current->type == AND)
		cmd->logic = CMD_AND;
	else if (current->type == OR)
		cmd->logic = CMD_OR;
	else if (current->type == PIPE)
		cmd->logic = CMD_PIPE;
	if (!create_cmd(new_cmd))
		return (0);
	cmd->next = *new_cmd;
	return (1);
}

int	handle_token_error(t_token *current, t_shell *shell)
{
	char	*token_value;

	token_value = "unexpected token";
	if (current && current->value)
		token_value = current->value;
	return (handle_syntax_error(shell, token_value));
}
