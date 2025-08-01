/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:29:28 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/15 17:30:36 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main error handler - central point for all error management */
int	handle_error(t_shell* shell, t_error_type type, const char* context)
{
	int exit_code;

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
	else if (type == ERR_NO_SUCH_FILE)
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
	else if (type == ERR_CANNOT_EXECUTE)
	{
		print_error("minishell", context, "cannot execute binary file");
		exit_code = EXIT_CANNOT_EXECUTE;
	}
	else if (type == ERR_MEMORY || type == ERR_MALLOC)
	{
		print_error("minishell", "malloc", "cannot allocate memory");
		exit_code = EXIT_FAILURE;
	}
	return (set_exit_status(shell, exit_code));
}

/* Handle system call errors using errno */
int	handle_system_error(t_shell* shell, const char* context)
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

/* Handle syntax errors */
int	handle_syntax_error(t_shell* shell, const char* token)
{
	char* error_msg;
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
int	handle_command_error(t_shell* shell, const char* cmd, int error_code)
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
void	print_error(const char* prefix, const char* context, const char* message)
{
	write(STDERR_FILENO, prefix, strlen(prefix));
	write(STDERR_FILENO, ": ", 2);
	if (context)
	{
		write(STDERR_FILENO, context, strlen(context));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, message, strlen(message));
	write(STDERR_FILENO, "\n", 1);
}

/* Set exit status and return it for convenience */
int	set_exit_status(t_shell* shell, int status)
{
	shell->last_exit_status = status;
	return (status);
}

/* CD command errors */
int	cd_error(t_shell* shell, const char* path, int error_type)
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
int	export_error(t_shell* shell, const char* var)
{
	print_error("minishell: export", var, "not a valid identifier");
	return (set_exit_status(shell, EXIT_FAILURE));
}

/* Unset command errors */
int	unset_error(t_shell* shell, const char* var)
{
	print_error("minishell: unset", var, "not a valid identifier");
	return (set_exit_status(shell, EXIT_FAILURE));
}

/* Exit command errors (non-numeric argument) */
int	exit_error(t_shell* shell, const char* arg)
{
	print_error("minishell: exit", arg, "numeric argument required");
	return (set_exit_status(shell, EXIT_MISUSE));
}

/* Other builtin errors */
int	echo_error(t_shell* shell)
{
	return (set_exit_status(shell, EXIT_FAILURE));
}

int	pwd_error(t_shell* shell)
{
	print_error("minishell: pwd", NULL, "error retrieving current directory");
	return (set_exit_status(shell, EXIT_FAILURE));
}

int	env_error(t_shell* shell)
{
	return (set_exit_status(shell, EXIT_FAILURE));
}