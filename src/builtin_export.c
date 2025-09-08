#include "minishell.h"

static void	print_export_format(char *env_var)
{
	char	*equals_pos;
	char	*var_name;
	char	*var_value;

	equals_pos = ft_strchr(env_var, '=');
	if (equals_pos)
	{
		var_name = ft_strndup(env_var, equals_pos - env_var);
		var_value = equals_pos + 1;
		printf("declare -x %s=\"%s\"\n", var_name, var_value);
		free(var_name);
	}
	else
	{
		printf("declare -x %s\n", env_var);
	}
}

static void	print_all_exports(t_shell *shell)
{
	int	i;

	if (!shell || !shell->env)
		return ;
	i = 0;
	while (shell->env[i])
	{
		print_export_format(shell->env[i]);
		i++;
	}
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;
	int	exit_status;

	if (!shell)
		return (EXIT_FAILURE);
	if (!argv[1])
	{
		print_all_exports(shell);
		return (set_exit_status(shell, EXIT_SUCCESS));
	}
	exit_status = EXIT_SUCCESS;
	i = 0;
	while (argv[++i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			export_error(shell, argv[i]);
			exit_status = EXIT_FAILURE;
		}
		else if (ft_strchr(argv[i], '='))
		{
			if (!set_env_var(shell, argv[i]))
				exit_status = EXIT_FAILURE;
		}
	}
	return (set_exit_status(shell, exit_status));
}
