/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:33:02 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:33:05 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ERROR_MANAGEMENT_H
# define ERROR_MANAGEMENT_H

# include <errno.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>

/* Exit codes following bash conventions */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_MISUSE 2
# define EXIT_CANNOT_EXECUTE 126
# define EXIT_COMMAND_NOT_FOUND 127
# define EXIT_SIGNAL_BASE 128

/* Error types */
typedef enum e_error_type
{
	ERR_NONE,
	ERR_SYNTAX,
	ERR_COMMAND_NOT_FOUND,
	ERR_NO_SUCH_FILE,
	ERR_PERMISSION_DENIED,
	ERR_IS_DIRECTORY,
	ERR_NOT_DIRECTORY,
	ERR_CANNOT_EXECUTE,
	ERR_MEMORY,
	ERR_PIPE,
	ERR_FORK,
	ERR_DUP,
	ERR_BUILTIN,
	ERR_REDIRECT,
	ERR_QUOTE,
	ERR_VARIABLE,
	ERR_CD,
	ERR_EXPORT,
	ERR_UNSET,
	ERR_EXIT_NUMERIC,
	ERR_UNCLOSED_QUOTE,
	ERR_MALLOC
} t_error_type;

/* Forward declaration for t_shell */
typedef struct s_shell t_shell;

/* Error handling functions */
int		handle_error(t_shell* shell, t_error_type type, const char* context);
int		handle_system_error(t_shell* shell, const char* context);
int		handle_syntax_error(t_shell* shell, const char* token);
int		handle_command_error(t_shell* shell, const char* cmd, int error_code);
void	print_error(const char* prefix, const char* context, const char* message);
int		set_exit_status(t_shell* shell, int status);

/* Builtin error handlers */
int		cd_error(t_shell* shell, const char* path, int error_type);
int		export_error(t_shell* shell, const char* var);
int		unset_error(t_shell* shell, const char* var);
int		exit_error(t_shell* shell, const char* arg);
int		echo_error(t_shell* shell);
int		pwd_error(t_shell* shell);
int		env_error(t_shell* shell);

#endif
