/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:28:45 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/02 12:19:16 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char* get_env_value(t_shell* shell, char* var_name)
{
	int	i;
	int	var_len;

	if (!shell || !shell->env || !var_name)
		return (NULL);

	fprintf(stderr, "DEBUG get_env_value: buscando '%s'\n", var_name);

	var_len = ft_strlen(var_name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PWD", 3) == 0)
			fprintf(stderr, "DEBUG get_env_value: comparando con '%s'\n", shell->env[i]);

		if (ft_strncmp(shell->env[i], var_name, var_len) == 0
			&& shell->env[i][var_len] == '=')
		{
			fprintf(stderr, "DEBUG get_env_value: ENCONTRADO en env[%d]='%s'\n",
				i, shell->env[i]);
			return (&shell->env[i][var_len + 1]);
		}
		i++;
	}

	fprintf(stderr, "DEBUG get_env_value: NO ENCONTRADO\n");
	return (NULL);
}

char	*get_special_var(t_shell *shell, char *var_name)
{
	if (!shell || !var_name)
		return (NULL);
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(shell->last_exit_status));
	else if (ft_strncmp(var_name, "$", 2) == 0)
		return (ft_itoa(getpid()));
	else if (ft_strncmp(var_name, "0", 2) == 0)
		return (ft_strdup("minishell"));
	return (NULL);
}

char* expand_variable(t_shell* shell, char* var_name)
{
	char* value;

	if (!shell || !var_name)
		return (NULL);

	fprintf(stderr, "DEBUG expand_variable: var_name='%s'\n", var_name);

	value = get_special_var(shell, var_name);
	if (value)
	{
		fprintf(stderr, "DEBUG expand_variable: special var value='%s'\n", value);
		return (value);
	}
	value = get_env_value(shell, var_name);

	fprintf(stderr, "DEBUG expand_variable: env value='%s'\n", value ? value : "NULL");

	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}
