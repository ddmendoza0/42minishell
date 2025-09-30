#include "minishell.h"

/* Set exit status and return it for convenience */
int	set_exit_status(t_shell *shell, int status)
{
	shell->last_exit_status = status;
	return (status);
}
