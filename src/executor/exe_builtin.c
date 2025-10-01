#include "minishell.h"
#include <sys/wait.h>

/*
 * BUILTIN COMMAND DETECTION
 */
static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (1);
	return (0);
}

/*
 * BUILTIN COMMAND DISPATCHER
 */
static int	execute_builtin(char **argv, t_shell *shell)
{
	char	*cmd;

	if (!argv || !argv[0] || !shell)
		return (EXIT_FAILURE);
	cmd = argv[0];
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (builtin_cd(argv, shell));
	else if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (builtin_pwd(argv, shell));
	else if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (builtin_echo(argv, shell));
	else if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (builtin_export(argv, shell));
	else if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (builtin_unset(argv, shell));
	else if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (builtin_env(argv, shell));
	else if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (builtin_exit(argv, shell));
	return (EXIT_FAILURE);
}
