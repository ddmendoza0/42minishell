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

char* get_error_message(t_error_code code)
{
	static char* messages[] = {
		[SUCCESS] = "Success",
		[ERR_MALLOC] = "Memory allocation failed",
		[ERR_OUT_OF_MEMORY] = "Out of memory",
		[ERR_UNCLOSED_QUOTE] = "syntax error: unclosed quote",
		[ERR_INVALID_TOKEN] = "syntax error: invalid token",
		[ERR_LEXER_FAILURE] = "lexer failure",
		[ERR_SYNTAX_ERROR] = "syntax error",
		[ERR_UNEXPECTED_TOKEN] = "syntax error: unexpected token",
		[ERR_MISSING_COMMAND] = "syntax error: missing command",
		[ERR_INVALID_PIPE] = "syntax error: invalid pipe",
		[ERR_INVALID_REDIRECT] = "syntax error: invalid redirection",
		[ERR_UNMATCHED_PARENTHESES] = "syntax error: unmatched parentheses",
		[ERR_COMMAND_NOT_FOUND] = "command not found",
		[ERR_PERMISSION_DENIED] = "Permission denied",
		[ERR_FILE_NOT_FOUND] = "No such file or directory",
		[ERR_IS_DIRECTORY] = "Is a directory",
		[ERR_NO_SUCH_FILE] = "No such file or directory",
		[ERR_PIPE_FAILED] = "pipe failed",
		[ERR_FORK_FAILED] = "fork failed",
		[ERR_EXEC_FAILED] = "execution failed",
		[ERR_REDIRECT_FAILED] = "redirection failed",
		[ERR_AMBIGUOUS_REDIRECT] = "ambiguous redirect",
		[ERR_BAD_FD] = "bad file descriptor",
		[ERR_BUILTIN_FAILED] = "builtin command failed",
		[ERR_CD_FAILED] = "cd: failed to change directory",
		[ERR_EXPORT_FAILED] = "export: failed",
		[ERR_UNSET_FAILED] = "unset: failed",
		[ERR_EXIT_INVALID_ARG] = "exit: numeric argument required",
		[ERR_ENV_VAR_NOT_FOUND] = "environment variable not found",
		[ERR_ENV_EXPANSION_FAILED] = "variable expansion failed",
		[ERR_GENERIC] = "unknown error"
	};

	if (code >= 0 && code < sizeof(messages) / sizeof(messages[0]) && messages[code])
		return (messages[code]);
	return ("unknown error");
}

// Get appropriate exit code for error
int get_exit_code(t_error_code code)
{
	if (code >= ERR_SYNTAX_ERROR && code <= ERR_UNMATCHED_PARENTHESES)
		return (2); // Syntax errors
	if (code == ERR_COMMAND_NOT_FOUND)
		return (127);
	if (code == ERR_PERMISSION_DENIED || code == ERR_IS_DIRECTORY)
		return (126);
	if (code >= ERR_BUILTIN_FAILED && code <= ERR_EXIT_INVALID_ARG)
		return (1); // Builtin errors
	if (code == SUCCESS)
		return (0);
	return (1); // Generic error exit code
}

// Set error in global state
void set_error(t_shell_state* shell, t_error_code code, char* context)
{
	if (!shell)
		return;

	// Clear previous error context
	if (shell->last_error.context)
	{
		free(shell->last_error.context);
		shell->last_error.context = NULL;
	}

	shell->last_error.code = code;
	shell->last_error.message = get_error_message(code);
	if (context)
		shell->last_error.context = ft_strdup(context);
	shell->exit_code = get_exit_code(code);
}

// Clear error state
void clear_error(t_shell_state* shell)
{
	if (!shell)
		return;

	if (shell->last_error.context)
	{
		free(shell->last_error.context);
		shell->last_error.context = NULL;
	}

	shell->last_error.code = SUCCESS;
	shell->last_error.message = NULL;
	shell->exit_code = 0;
}

// Check if there's an active error
int has_error(t_shell_state* shell)
{
	if (!shell)
		return (0);
	return (shell->last_error.code != SUCCESS);
}

// Print error to stderr in bash-like format
void print_error(t_shell_state* shell)
{
	if (!shell || !has_error(shell))
		return;

	if (shell->last_error.context)
		fprintf(stderr, "minishell: %s: %s\n",
			shell->last_error.context, shell->last_error.message);
	else
		fprintf(stderr, "minishell: %s\n", shell->last_error.message);
}

// Initialize shell state
void init_shell_state(t_shell_state* shell)
{
	if (!shell)
		return;

	shell->last_error.code = SUCCESS;
	shell->last_error.message = NULL;
	shell->last_error.context = NULL;
	shell->last_error.line = 0;
	shell->last_error.column = 0;
	shell->exit_code = 0;
}

// Cleanup shell state
void cleanup_shell_state(t_shell_state* shell)
{
	if (!shell)
		return;

	if (shell->last_error.context)
	{
		free(shell->last_error.context);
		shell->last_error.context = NULL;
	}
}
