/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:45:28 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 19:48:32 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static char	**remove_empty_args(char **argv)
{
	char	**new_argv;
	int		i;
	int		j;
	int		count;

	if (!argv)
		return (NULL);
	count = 0;
	i = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			count++;
		i++;
	}
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

int	execute_single_command(t_command *cmd, t_shell *shell)
{
	char	**argv;
	char	**clean_argv;
	int		saved_stdin;
	int		saved_stdout;
	int		exit_status;

	if (!cmd || !shell)
		return (EXIT_FAILURE);
	if (cmd->subshell)
		return (execute_command_tree(cmd->subshell, shell));
	if (!prepare_argv(cmd, &argv))
		return (EXIT_SUCCESS);
	clean_argv = remove_empty_args(argv);
	free_argv(argv);
	if (!clean_argv || !clean_argv[0])
	{
		if (clean_argv)
			free_argv(clean_argv);
		return (set_exit_status(shell, EXIT_SUCCESS));
	}
	if (!stp_redir(cmd, &saved_stdin, &saved_stdout, shell))
	{
		free_argv(clean_argv);
		return (EXIT_FAILURE);
	}
	if (is_builtin(clean_argv[0]))
		exit_status = execute_builtin(clean_argv, shell);
	else
		exit_status = execute_external(clean_argv, shell);
	restore_redirections(saved_stdin, saved_stdout);
	free_argv(clean_argv);
	return (set_exit_status(shell, exit_status));
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
