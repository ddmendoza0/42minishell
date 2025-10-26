/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_logic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:45:15 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/26 13:39:10 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*static int	should_stop_execution(t_command *exed_cmd, int exit_status)
{
	if (!exed_cmd || !exed_cmd->next)
		return (1);
	if (exed_cmd->logic == CMD_AND && exit_status != EXIT_SUCCESS)
		return (1);
	if (exed_cmd->logic == CMD_OR && exit_status == EXIT_SUCCESS)
		return (1);
	return (0);
}*/

int	execute_logical_sequence(t_command *cmd_list, t_shell *shell)
{
	t_command	*current;
	t_command	*pipeline_end;
	int			exit_status;
	int			should_exe;

	current = cmd_list;
	exit_status = EXIT_SUCCESS;
	should_exe = 1;
	while (current)
	{
		if (should_exe)
		{
			if (current->logic == CMD_PIPE)
			{
				pipeline_end = find_pipeline_end(current);
				exit_status = execute_pipeline(current, shell);
				current = pipeline_end->next;
			}
			else
				exit_status = execute_single_command(current, shell);
		}
		if (current->logic == CMD_AND)
			should_exe = (exit_status == EXIT_SUCCESS);
		else if (current->logic == CMD_OR)
			should_exe = (exit_status != EXIT_SUCCESS);
		else
			should_exe = 1;
		current = current->next;
	}
	return (exit_status);
}
