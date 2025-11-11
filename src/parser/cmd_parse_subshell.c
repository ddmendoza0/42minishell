/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse_subshell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:45:01 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 14:45:19 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_to_end_or_rparen(t_token **current)
{
	while (*current)
	{
		if ((*current)->type == RPAREN)
		{
			*current = (*current)->next;
			return ;
		}
		*current = (*current)->next;
	}
}

static int	count_parenthesis(t_token *start)
{
	t_token	*end;
	int		paren_count;

	end = start;
	paren_count = 1;
	while (end && paren_count > 0)
	{
		if (end->type == LPAREN)
			paren_count++;
		else if (end->type == RPAREN)
			paren_count--;
		if (paren_count > 0)
			end = end->next;
	}
	return (paren_count);
}

t_token	*extract_subshell_tokens(t_token **current)
{
	t_token	*start;
	t_token	*end;
	t_token	*after;

	start = (*current)->next;
	if (count_parenthesis(start) != 0)
	{
		skip_to_end_or_rparen(current);
		return (NULL);
	}
	end = start;
	while (end && end->next && end->next->type != RPAREN)
		end = end->next;
	if (end && end->next)
	{
		after = end->next->next;
		end->next = NULL;
	}
	else
		after = NULL;
	*current = after;
	return (start);
}

int	handle_lparen(t_command *cmd, t_token **current, t_shell *shell)
{
	t_token	*sub_tokens;

	sub_tokens = extract_subshell_tokens(current);
	if (!sub_tokens)
		return (handle_syntax_error(shell, "unmatched parenthesis"));
	cmd->subshell = cmd_builder(&sub_tokens, shell);
	return (cmd->subshell != NULL);
}
