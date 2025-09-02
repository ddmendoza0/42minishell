#include "minishell.h"

int	builtin_pwd(char **argv, t_shell *shell)
{
	char	*cwd;

	if (!shell)
		return (pwd_error(shell));
	if (argv[1])
	{
		print_error("minishell: pwd", NULL, "too many arguments");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (shell->cwd)
		{
			printf("%s\n", shell->cwd);
			return (set_exit_status(shell, EXIT_SUCCESS));
		}
		else
			return (pwd_error(shell));
	}
	printf("%s\n", cwd);
	if (!shell->cwd || ft_strncmp(shell->cwd, cwd, ft_strlen(cwd) + 1) != 0)
	{
		if (shell->cwd)
			free(shell->cwd);
		shell->cwd = ft_strdup(cwd);
	}
	free(cwd);
	return (set_exit_status(shell, EXIT_SUCCESS));
}
