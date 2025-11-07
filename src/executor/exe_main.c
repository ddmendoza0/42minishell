/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:45:28 by dmendoza          #+#    #+#             */
/*   Updated: 2025/11/07 10:37:45 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static int	count_non_empty_args(char **argv)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

char	**remove_empty_args(char **argv)
{
	char	**new_argv;
	int		i;
	int		j;
	int		count;

	if (!argv)
		return (NULL);
	count = count_non_empty_args(argv);
	if (count == 0)
		return (NULL);
	new_argv = malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
		return (NULL);
	i = 0;
	j = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			new_argv[j++] = ft_strdup(argv[i]);
		i++;
	}
	new_argv[j] = NULL;
	return (new_argv);
}

/*
 * MAIN EXECUTION DISPATCHER
 */
int	execute_command_tree(t_command *cmd_tree, t_shell *shell)
{
	if (!cmd_tree || !shell)
		return (EXIT_FAILURE);
	if (has_pipeline(cmd_tree))
		return (execute_pipeline(cmd_tree, shell));
	else if (has_logical_ops(cmd_tree))
		return (execute_logical_sequence(cmd_tree, shell));
	else
		return (execute_single_command(cmd_tree, shell));
}
