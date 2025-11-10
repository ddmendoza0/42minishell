/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:26:37 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:26:38 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	is_valid_var(const char *str)
{
    int i;

    if (!str || !ft_isalpha(str[0]))
        return 0;
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	if (!is_valid_identifier(arg))
	{
		export_error(shell, arg);
		return (EXIT_FAILURE);
	}
	else if (ft_strchr(arg, '='))
	{
		if (!set_env_var(shell, arg))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;
	char *eq;
	char *name;

	if (!shell)
		return (EXIT_FAILURE);
	if (!argv[1])
	{
		print_all_exports(shell);
		return (set_exit_status(shell, EXIT_SUCCESS));
	}
	i = 1;
	while (argv[i])
	{
		if (!is_valid_var(argv[i]))
		{
			handle_error(shell, ERR_INVALID_IDENTIFIER, argv[i]);
		}
		else
		{
			eq = ft_strchr(argv[i], '=');
			if (eq)
				set_env_var(argv[i]);
			else
			{
				if (getenv(argv[i]) == NULL)
				{
					name = ft_strjoin(argv[i], "=");
					if (!name)
					{
						handle_error(shell, ERR_MALLOC, "export variable");
						return set_exit_status(shell, EXIT_FAILURE);
					}
					set_env_var(name);
					free(name);
				}
			}
		}
		i++;
	}
	return (set_exit_status(shell, EXIT_SUCCESS));
}
