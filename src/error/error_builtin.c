/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:43:02 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:43:05 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* CD command errors */
int	cd_error(t_shell *shell, const char *path, int error_type)
{
	if (error_type == ENOENT)
		print_error("minishell: cd", path, "No such file or directory");
	else if (error_type == ENOTDIR)
		print_error("minishell: cd", path, "Not a directory");
	else if (error_type == EACCES)
		print_error("minishell: cd", path, "Permission denied");
	else
		print_error("minishell: cd", path, strerror(error_type));
	return (set_exit_status(shell, EXIT_FAILURE));
}

/* Export command errors */
int	export_error(t_shell *shell, const char *var)
{
	print_error("minishell: export", var, "not a valid identifier");
	return (set_exit_status(shell, EXIT_FAILURE));
}

/* Unset command errors */
int	unset_error(t_shell *shell, const char *var)
{
	print_error("minishell: unset", var, "not a valid identifier");
	return (set_exit_status(shell, EXIT_FAILURE));
}

/* Exit command errors (non-numeric argument) */
int	exit_error(t_shell *shell, const char *arg)
{
	print_error("minishell: exit", arg, "numeric argument required");
	return (set_exit_status(shell, EXIT_MISUSE));
}

int	pwd_error(t_shell *shell)
{
	print_error("minishell: pwd", NULL, "error retrieving current directory");
	return (set_exit_status(shell, EXIT_FAILURE));
}
