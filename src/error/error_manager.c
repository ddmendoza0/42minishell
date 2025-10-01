/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:29:28 by dmendoza          #+#    #+#             */
/*   Updated: 2025/09/30 13:36:53 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle syntax and command errors */
static int	handle_syntax_command_errors(t_error_type type, const char *context)
{
	int	exit_code;

	exit_code = EXIT_FAILURE;
	if (type == ERR_SYNTAX || type == ERR_UNCLOSED_QUOTE)
	{
		print_error("minishell", context, "syntax error");
		exit_code = EXIT_MISUSE;
	}
	else if (type == ERR_COMMAND_NOT_FOUND)
	{
		print_error("minishell", context, "command not found");
		exit_code = EXIT_COMMAND_NOT_FOUND;
	}
	return (exit_code);
}

/* Handle file and permission errors */
static int	handle_file_errors(t_error_type type, const char *context)
{
	int	exit_code;

	exit_code = EXIT_FAILURE;
	if (type == ERR_NO_SUCH_FILE)
	{
		print_error("minishell", context, "No such file or directory");
		exit_code = EXIT_FAILURE;
	}
	else if (type == ERR_PERMISSION_DENIED)
	{
		print_error("minishell", context, "Permission denied");
		exit_code = EXIT_CANNOT_EXECUTE;
	}
	else if (type == ERR_IS_DIRECTORY)
	{
		print_error("minishell", context, "Is a directory");
		exit_code = EXIT_CANNOT_EXECUTE;
	}
	return (exit_code);
}

/* Handle execution and memory errors */
static int	handle_exec_memory_errors(t_error_type type, const char *context)
{
	int	exit_code;

	exit_code = EXIT_FAILURE;
	if (type == ERR_CANNOT_EXECUTE)
	{
		print_error("minishell", context, "cannot execute binary file");
		exit_code = EXIT_CANNOT_EXECUTE;
	}
	else if (type == ERR_MEMORY || type == ERR_MALLOC)
	{
		print_error("minishell", "malloc", "cannot allocate memory");
		exit_code = EXIT_FAILURE;
	}
	return (exit_code);
}

/* Main error handler - central point for all error management */
int	handle_error(t_shell *shell, t_error_type type, const char *context)
{
	int	exit_code;

	exit_code = EXIT_FAILURE;
	if (type == ERR_SYNTAX || type == ERR_UNCLOSED_QUOTE || type == ERR_COMMAND_NOT_FOUND)
		exit_code = handle_syntax_command_errors(type, context);
	else if (type == ERR_NO_SUCH_FILE || type == ERR_PERMISSION_DENIED || type == ERR_IS_DIRECTORY)
		exit_code = handle_file_errors(type, context);
	else if (type == ERR_CANNOT_EXECUTE || type == ERR_MEMORY || type == ERR_MALLOC)
		exit_code = handle_exec_memory_errors(type, context);
	else if (type == ERR_BUILTIN)
	{
		print_error("minishell", context, "error");
		exit_code = EXIT_FAILURE;
	}
	return (set_exit_status(shell, exit_code));
}

/* Handle system call errors using errno */
int	handle_system_error(t_shell *shell, const char *context)
{
	int	exit_code;

	exit_code = EXIT_FAILURE;
	if (errno == ENOENT)
		exit_code = handle_error(shell, ERR_NO_SUCH_FILE, context);
	else if (errno == EACCES)
		exit_code = handle_error(shell, ERR_PERMISSION_DENIED, context);
	else if (errno == EISDIR)
		exit_code = handle_error(shell, ERR_IS_DIRECTORY, context);
	else if (errno == ENOTDIR)
		exit_code = handle_error(shell, ERR_NOT_DIRECTORY, context);
	else if (errno == ENOMEM)
		exit_code = handle_error(shell, ERR_MEMORY, context);
	else
	{
		print_error("minishell", context, strerror(errno));
		exit_code = EXIT_FAILURE;
	}
	return (set_exit_status(shell, exit_code));
}
