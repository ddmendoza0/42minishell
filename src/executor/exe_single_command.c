/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_single_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:35:53 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/26 13:10:12 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static int	prepare_clean_argv(t_command *cmd, char ***clean_argv)
{
	char	**argv;

	if (!prepare_argv(cmd, &argv))
		return (0);
	*clean_argv = remove_empty_args(argv);
	free_argv(argv);
	if (!(*clean_argv) || !(*clean_argv)[0])
	{
		if (*clean_argv)
			free_argv(*clean_argv);
		return (0);
	}
	return (1);
}

static int	execute_with_redirections(char **clean_argv, t_command *cmd,
		t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	exit_status;

	if (!stp_redir(cmd, &saved_stdin, &saved_stdout, shell))
	{
		free_argv(clean_argv);
		return (EXIT_FAILURE);
	}
	if (is_builtin(clean_argv[0]))
		exit_status = execute_builtin(clean_argv, cmd, shell);
	else
		exit_status = execute_external(clean_argv, shell);
	restore_redirections(saved_stdin, saved_stdout);
	free_argv(clean_argv);
	return (set_exit_status(shell, exit_status));
}

int	execute_single_command(t_command *cmd, t_shell *shell)
{
	char	**clean_argv;

	if (!cmd || !shell)
		return (EXIT_FAILURE);
	if (cmd->subshell)
		return (execute_command_tree(cmd->subshell, shell));
	if (!prepare_clean_argv(cmd, &clean_argv))
		return (set_exit_status(shell, EXIT_SUCCESS));
	return (execute_with_redirections(clean_argv, cmd, shell));
}
