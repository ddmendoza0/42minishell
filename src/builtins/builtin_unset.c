/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:27:05 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 15:44:38 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_env_var_index(t_shell *shell, char *var_name)
{
	int	i;
	int	var_len;

	if (!shell || !shell->env || !var_name)
		return (-1);
	var_len = ft_strlen(var_name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var_name, var_len)
			== 0 && shell->env[i][var_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	remove_env_var(t_shell *shell, int index)
{
	int	env_count;
	int	i;

	env_count = 0;
	while (shell->env[env_count])
		env_count++;
	free(shell->env[index]);
	i = index;
	while (i < env_count - 1)
	{
		shell->env[i] = shell->env[i + 1];
		i++;
	}
	shell->env[env_count - 1] = NULL;
	return (1);
}

static int	is_valid_unset_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;
	int	index;
	int	exit_status;

	if (!shell)
		return (EXIT_FAILURE);
	if (!argv[1])
		return (set_exit_status(shell, EXIT_SUCCESS));
	exit_status = EXIT_SUCCESS;
	i = 0;
	while (argv[++i])
	{
		if (!is_valid_unset_identifier(argv[i]))
		{
			unset_error(shell, argv[i]);
			exit_status = EXIT_FAILURE;
		}
		else
		{
			index = find_env_var_index(shell, argv[i]);
			if (index != -1)
				remove_env_var(shell, index);
		}
	}
	return (set_exit_status(shell, exit_status));
}
