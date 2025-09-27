#include "minishell.h"

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
		return (NULL);
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

int	handle_lparen(t_command *cmd, t_token **current)
{
	t_token	*sub_tokens;

	sub_tokens = extract_subshell_tokens(current);
	if (!sub_tokens)
	{
		printf("Syntax error: unmatched parenthesis\n");
		return (0);
	}
	cmd->subshell = cmd_builder(&sub_tokens);
	return (cmd->subshell != NULL);
}
