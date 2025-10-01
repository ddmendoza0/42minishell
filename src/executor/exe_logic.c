#include "minishell.h"
#include <sys/wait.h>

static t_command	*find_pipeline_end(t_command *start)
{
	t_command	*end;

	end = start;
	while (end->next && end->logic == CMD_PIPE)
		end = end->next;
	return (end);
}

static int	should_stop_execution(t_command *current, int exit_status)
{
	if (!current)
		return (1);
	if (current->logic == CMD_AND && exit_status != EXIT_SUCCESS)
		return (1);
	if (current->logic == CMD_OR && exit_status == EXIT_SUCCESS)
		return (1);
	return (0);
}

int	execute_logical_sequence(t_command *cmd_list, t_shell *shell)
{
	t_command	*current;
	t_command	*pipeline_end;
	int			exit_status;

	current = cmd_list;
	exit_status = EXIT_SUCCESS;
	while (current)
	{
		if (current->logic == CMD_PIPE)
		{
			pipeline_end = find_pipeline_end(current);
			exit_status = execute_pipeline(current, shell);
			current = pipeline_end->next;
		}
		else
		{
			exit_status = execute_single_command(current, shell);
			current = current->next;
		}
		if (should_stop_execution(current, exit_status))
			break ;
	}
	return (exit_status);
}
