#include "minishell.h"

t_arg_token	*create_arg_token(t_token *original)
{
	t_arg_token	*arg_token;

	if (!original)
		return (NULL);
	arg_token = malloc(sizeof(t_arg_token));
	if (!arg_token)
		return (NULL);
	arg_token->original_token = original;
	arg_token->expanded_value = NULL;
	arg_token->next = NULL;
	return (arg_token);
}

void	add_arg_token(t_arg_token **list, t_arg_token *new_arg)
{
	t_arg_token	*current;

	if (!list || !new_arg)
		return ;
	if (!*list)
	{
		*list = new_arg;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_arg;
}

void	free_arg_tokens(t_arg_token *args)
{
	t_arg_token	*temp;

	while (args)
	{
		temp = args->next;
		if (args->expanded_value)
			free(args->expanded_value);
		free(args);
		args = temp;
	}
}

int	add_token_to_args(t_command *cmd, t_token *token)
{
	t_arg_token	*arg_token;

	if (!cmd || !token)
		return (0);
	arg_token = create_arg_token(token);
	if (!arg_token)
		return (0);
	add_arg_token(&cmd->args, arg_token);
	return (1);
}
