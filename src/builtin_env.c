#include "minishell.h"

int	builtin_env(char **argv, t_shell *shell)
{
	int	i;

	if (!shell)
		return (env_error(shell));
	if (argv[1])
	{
		print_error("minishell: env", NULL, "too many arguments");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	if (!shell->env)
		return (env_error(shell));
	i = 0;
	while (shell->env[i])
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	return (set_exit_status(shell, EXIT_SUCCESS));
}
