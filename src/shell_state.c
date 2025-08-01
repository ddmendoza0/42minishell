
#include "minishell.h"

void	cleanup_shell(t_shell* shell)
{
	int	i;

	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}

	if (shell->cwd)
		free(shell->cwd);

	if (shell->stdin_backup >= 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup >= 0)
		close(shell->stdout_backup);
}

static int	count_env_vars(char** envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static int	copy_environment(t_shell* shell, char** envp)
{
	int	env_count;
	int	i;

	env_count = count_env_vars(envp);
	shell->env = malloc(sizeof(char*) * (env_count + 1));
	if (!shell->env)
		return (0);
	i = 0;
	while (i < env_count)
	{
		shell->env[i] = ft_strdup(envp[i]);
		if (!shell->env[i])
		{
			while (--i >= 0)
				free(shell->env[i]);
			free(shell->env);
			shell->env = NULL;
			return (0);
		}
		i++;
	}
	shell->env[i] = NULL;
	return (1);
}

int	init_shell(t_shell* shell, char** envp)
{
	if (!copy_environment(shell, envp))
	{
		perror("minishell: malloc");
		return (0);
	}
	shell->cwd = getcwd(NULL, 0);
	if (!shell->cwd)
	{
		perror("minishell: getcwd");
		cleanup_shell(shell);
		return (0);
	}
	shell->last_exit_status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	if (shell->stdin_backup == -1 || shell->stdout_backup == -1)
	{
		perror("minishell: dup");
		cleanup_shell(shell);
		return (0);
	}
	return (1);
}
