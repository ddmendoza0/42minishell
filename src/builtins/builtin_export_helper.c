/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:26:32 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/11/12 16:27:16 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_variable_without_value(char *arg, t_shell *shell)
{
	char	*name;

	if (getenv(arg) == NULL)
	{
		name = ft_strjoin(arg, "=");
		if (!name)
		{
			handle_error(shell, ERR_MALLOC, "export variable");
			return (EXIT_FAILURE);
		}
		set_env_var(shell, name);
		free(name);
	}
	return (EXIT_SUCCESS);
}
