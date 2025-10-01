#include "minishell.h"

void	handle_interactive_signals(t_shell *shell)
{
	if (g_signal_received == SIGINT)
	{
		shell->last_exit_status = 130;
		g_signal_received = 0;
	}
}

int	get_signal_exit_status(int sig)
{
	return (128 + sig);
}
