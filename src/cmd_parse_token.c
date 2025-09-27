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

int	handle_syntax_error(t_token *current)
{
	printf("Syntax error: unexpected token");
	if (current->value)
		printf(" '%s'", current->value);
	printf("\n");
	return (0);
}
