/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:43:32 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:43:34 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Set exit status and return it for convenience */
int	set_exit_status(t_shell *shell, int status)
{
	shell->last_exit_status = status;
	return (status);
}

/* Handle syntax errors */
int	handle_syntax_error(t_shell *shell, const char *token)
{
	char	*error_msg;
	int		exit_code;

	if (!token)
		error_msg = "syntax error near unexpected token `newline'";
	else
	{
		error_msg = malloc(strlen(token) + 50);
		if (!error_msg)
			return (handle_error(shell, ERR_MEMORY, "syntax error"));
		sprintf(error_msg, "syntax error near unexpected token `%s'", token);
	}
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, error_msg, strlen(error_msg));
	write(STDERR_FILENO, "\n", 1);
	if (token)
		free(error_msg);
	exit_code = EXIT_MISUSE;
	return (set_exit_status(shell, exit_code));
}

/* Handle command execution errors */
int	handle_command_error(t_shell *shell, const char *cmd, int error_code)
{
	if (error_code == ENOENT)
		return (handle_error(shell, ERR_COMMAND_NOT_FOUND, cmd));
	else if (error_code == EACCES)
		return (handle_error(shell, ERR_PERMISSION_DENIED, cmd));
	else if (error_code == EISDIR)
		return (handle_error(shell, ERR_IS_DIRECTORY, cmd));
	else
		return (handle_system_error(shell, cmd));
}

/* Generic error printer following bash format */
void	print_error(const char *prefix,	const char *context, const char *msg)
{
	write(STDERR_FILENO, prefix, strlen(prefix));
	write(STDERR_FILENO, ": ", 2);
	if (context)
	{
		write(STDERR_FILENO, context, strlen(context));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}
