/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:02:36 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/11/12 17:02:39 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection_token(t_command *cmd, t_token **current,
				t_shell *shell)
{
	if ((*current)->type == REDIRECT_IN || (*current)->type == HEREDOC)
		return (add_token_redir_in(cmd, current, shell));
	else if ((*current)->type == REDIRECT_OUT || (*current)->type == APPEND_OUT)
		return (add_token_redir_out(cmd, current, shell));
	return (0);
}

int	handle_logic_operator(t_command **cmd, t_token **current,
				t_shell *shell)
{
	t_command	*new_cmd;

	if (!handle_logic_token(*cmd, *current, &new_cmd))
		return (0);
	*cmd = new_cmd;
	*current = (*current)->next;
	if (*current && ((*current)->type == PIPE || (*current)->type == AND
			|| (*current)->type == OR))
		return (handle_syntax_error(shell, (*current)->value));
	return (1);
}
