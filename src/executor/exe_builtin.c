/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:44:07 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/26 13:14:56 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*
 * BUILTIN COMMAND DETECTION
 */
int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/*
 * BUILTIN COMMAND DISPATCHER
 */
int	execute_builtin(char **argv, t_command *cmd, t_shell *shell)
{
	char	*cmd_name;

	if (!argv || !argv[0] || !shell)
		return (EXIT_FAILURE);
	cmd_name = argv[0];
	if (ft_strncmp(cmd_name, "cd", 3) == 0 && cmd_name[2] == '\0')
		return (builtin_cd(argv, shell));
	else if (ft_strncmp(cmd_name, "pwd", 4) == 0 && cmd_name[3] == '\0')
		return (builtin_pwd(argv, shell));
	else if (ft_strncmp(cmd_name, "echo", 5) == 0 && cmd_name[4] == '\0')
		return (builtin_echo(argv, cmd, shell));
	else if (ft_strncmp(cmd_name, "export", 7) == 0 && cmd_name[6] == '\0')
		return (builtin_export(argv, shell));
	else if (ft_strncmp(cmd_name, "unset", 6) == 0 && cmd_name[5] == '\0')
		return (builtin_unset(argv, shell));
	else if (ft_strncmp(cmd_name, "env", 4) == 0 && cmd_name[3] == '\0')
		return (builtin_env(argv, shell));
	else if (ft_strncmp(cmd_name, "exit", 5) == 0 && cmd_name[4] == '\0')
		return (builtin_exit(argv, shell));
	return (EXIT_FAILURE);
}
