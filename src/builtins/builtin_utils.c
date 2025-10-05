/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:27:18 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 15:45:10 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * FIND EXISTING VARIABLE INDEX
 * Returns index of existing variable or -1 if not found
 */
static int	find_env_var(char **env, char *var_name, int var_name_len)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, var_name_len)
			== 0 && env[i][var_name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	replace_env_var(t_shell *shell, int index, char*var_assignment)
{
	free(shell->env[index]);
	shell->env[index] = ft_strdup(var_assignment);
	return (shell->env[index] != NULL);
}

static int	add_new_env_var(t_shell	*shell, char *var_assignment, int env_count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (env_count + 2));
	if (!new_env)
		return (0);
	i = 0;
	while (i < env_count)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[env_count] = ft_strdup(var_assignment);
	new_env[env_count + 1] = NULL;
	if (!new_env[env_count])
	{
		free(new_env);
		return (0);
	}
	free(shell->env);
	shell->env = new_env;
	return (1);
}

static int	count_env_vars(char **env)
{
	int	count;

	count = 0;
	while (env[count])
		count++;
	return (count);
}

int	set_env_var(t_shell* shell, char* var_assignment)
{
	char* var_name;
	char* equals_pos;
	int		var_name_len;
	int		existing_index;

	if (!shell || !var_assignment)
		return (0);

	fprintf(stderr, "DEBUG set_env_var llamado con: %s\n", var_assignment);

	equals_pos = ft_strchr(var_assignment, '=');
	if (!equals_pos)
		return (0);
	var_name_len = equals_pos - var_assignment;
	var_name = ft_strndup(var_assignment, var_name_len);
	if (!var_name)
		return (0);

	fprintf(stderr, "DEBUG buscando variable: '%s' (len=%d)\n", var_name, var_name_len);

	existing_index = find_env_var(shell->env, var_name, var_name_len);

	fprintf(stderr, "DEBUG existing_index: %d\n", existing_index);

	if (existing_index != -1)
	{
		fprintf(stderr, "DEBUG reemplazando: '%s' -> '%s'\n",
			shell->env[existing_index], var_assignment);
		free(var_name);
		return (replace_env_var(shell, existing_index, var_assignment));
	}

	fprintf(stderr, "DEBUG anadiendo nueva variable\n");
	free(var_name);
	return (add_new_env_var(shell, var_assignment, count_env_vars(shell->env)));
}
