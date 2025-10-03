/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:45:28 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:45:30 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*
 * SINGLE COMMAND EXECUTION
 */
int	execute_single_command(t_command *cmd, t_shell *shell)
{
	char	**argv;
	int		saved_stdin;
	int		saved_stdout;
	int		exit_status;

	if (!cmd || !shell)
		return (EXIT_FAILURE);
	if (cmd->subshell)
		return (execute_command_tree(cmd->subshell, shell));
	if (!prepare_argv(cmd, &argv))
		return (EXIT_SUCCESS);
	if (!stp_redir(cmd, &saved_stdin, &saved_stdout, shell))
	{
		free_argv(argv);
		return (EXIT_FAILURE);
	}
	if (is_builtin(argv[0]))
		exit_status = execute_builtin(argv, shell);
	else
		exit_status = execute_external(argv, shell);
	restore_redirections(saved_stdin, saved_stdout);
	free_argv(argv);
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
