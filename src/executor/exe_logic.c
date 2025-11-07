/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_logic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:45:15 by dmendoza          #+#    #+#             */
/*   Updated: 2025/11/07 10:35:51 by dmendoza         ###   ########.fr       */
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

static int	should_execute_next(int exit_status, t_cmd_logic logic)
{
	if (logic == CMD_AND)
		return (exit_status == EXIT_SUCCESS);
	else if (logic == CMD_OR)
		return (exit_status != EXIT_SUCCESS);
	return (1);
}

static int	execute_command_block(t_command *current, t_shell *shell,
				t_command **next)
{
	t_command	*pipeline_end;
	int			exit_status;

	if (current->logic == CMD_PIPE)
	{
		pipeline_end = find_pipeline_end(current);
		exit_status = execute_pipeline(current, shell);
		*next = pipeline_end->next;
	}
	else
	{
		exit_status = execute_single_command(current, shell);
		*next = current->next;
	}
	return (exit_status);
}

int	execute_logical_sequence(t_command *cmd_list, t_shell *shell)
{
	t_command	*current;
	t_command	*next;
	int			exit_status;
	int			should_exe;

	current = cmd_list;
	exit_status = EXIT_SUCCESS;
	should_exe = 1;
	while (current)
	{
		if (should_exe)
			exit_status = execute_command_block(current, shell, &next);
		else
			next = current->next;
		should_exe = should_execute_next(exit_status, current->logic);
		current = next;
	}
	return (exit_status);
}
