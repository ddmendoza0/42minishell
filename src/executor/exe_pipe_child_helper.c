/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipe_child_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:50:22 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/11/12 16:50:25 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

char	*get_and_validate_path(char **argv, t_shell *shell)
{
	char	*executable_path;

	executable_path = find_executable(argv[0], shell);
	if (!executable_path)
	{
		if (ft_strchr(argv[0], '/'))
			handle_error(shell, ERR_NO_SUCH_FILE, argv[0]);
		else
			handle_error(shell, ERR_COMMAND_NOT_FOUND, argv[0]);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
	return (executable_path);
}
