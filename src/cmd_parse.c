/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:27:58 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 13:22:09 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_list(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
			free_arg_tokens(cmd->args);
		if (cmd->input_redir)
		{
			if (cmd->input_redir->fd >= 0)
				close(cmd->input_redir->fd);
			free_redir_file(cmd->input_redir);
		}
		if (cmd->output_redir)
		{
			if (cmd->output_redir->fd >= 0)
				close(cmd->output_redir->fd);
			free_redir_file(cmd->output_redir);
		}
		if (cmd->subshell)
			free_cmd_list(cmd->subshell);
		free(cmd);
		cmd = tmp;
	}
}

int	create_cmd(t_command **cmd)
{
	*cmd = malloc(sizeof(t_command));
	if (!*cmd)
		return (0);
	(*cmd)->args = NULL;
	(*cmd)->input_redir = NULL;
	(*cmd)->output_redir = NULL;
	(*cmd)->logic = CMD_NONE;
	(*cmd)->subshell = NULL;
	(*cmd)->next = NULL;
	return (1);
}

static int	process_token(t_command **cmd, t_token **current, t_shell *shell)
{
	t_command	*new_cmd;

	if ((*current)->type == WORD)
		return (handle_word_token(*cmd, current));
	else if ((*current)->type == REDIRECT_IN || (*current)->type == HEREDOC)
		return (add_token_redir_in(*cmd, current));
	else if ((*current)->type == REDIRECT_OUT || (*current)->type == APPEND_OUT)
		return (add_token_redir_out(*cmd, current));
	else if ((*current)->type == PIPE || (*current)->type == AND
		|| (*current)->type == OR)
	{
		if (!handle_logic_token(*cmd, *current, &new_cmd))
			return (0);
		*cmd = new_cmd;
		*current = (*current)->next;
		return (1);
	}
	else if ((*current)->type == LPAREN)
		return (handle_lparen(*cmd, current, shell));
	else if ((*current)->type == RPAREN)
	{
		*current = (*current)->next;
		return (1);
	}
	return (handle_token_error(*current, shell));
}

static int	cmd_parse_tokens(t_command *cmd, t_token *current, t_shell *shell)
{
	while (current)
	{
		if (!process_token(&cmd, &current, shell))
			return (0);
	}
	return (1);
}

t_command	*cmd_builder(t_token **tkn_list, t_shell *shell)
{
	t_command	*cmd;
	t_command	*head;
	t_token		*current;

	if (!tkn_list || !*tkn_list)
		return (NULL);
	current = *tkn_list;
	if (!create_cmd(&cmd))
		return (NULL);
	head = cmd;
	if (!cmd_parse_tokens(cmd, current, shell))
	{
		free_cmd_list(head);
		return (NULL);
	}
	return (head);
}
