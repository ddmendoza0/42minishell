#include "minishell.h"

/* Other builtin errors */
int	echo_error(t_shell *shell)
{
	return (set_exit_status(shell, EXIT_FAILURE));
}

int	pwd_error(t_shell *shell)
{
	print_error("minishell: pwd", NULL, "error retrieving current directory");
	return (set_exit_status(shell, EXIT_FAILURE));
}

int	env_error(t_shell *shell)
{
	return (set_exit_status(shell, EXIT_FAILURE));
}
