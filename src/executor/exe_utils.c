/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:47:54 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:47:55 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*
 * UTILITY FUNCTIONS
 */
void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

int	has_pipeline(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->logic == CMD_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}

int	has_logical_ops(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->logic == CMD_AND || current->logic == CMD_OR)
			return (1);
		current = current->next;
	}
	return (0);
}

int	count_pipeline_commands(t_command *cmd_list)
{
	t_command	*current;
	int			count;

	current = cmd_list;
	count = 0;
	while (current)
	{
		count++;
		if (current->logic != CMD_PIPE)
			break ;
		current = current->next;
	}
	return (count);
}

int	prepare_argv(t_command *cmd, char ***argv)
{
	*argv = get_argv_from_args(cmd);
	if (!*argv || !(*argv)[0])
	{
		if (*argv)
			free_argv(*argv);
		return (0);
	}
	return (1);
}
