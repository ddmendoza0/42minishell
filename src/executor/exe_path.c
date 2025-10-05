/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:45:47 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:45:49 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*
 * PATH RESOLUTION HELPERS
 */
static char	*create_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*result;

	if (!dir || !cmd)
		return (NULL);
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, cmd);
	free(temp);
	return (result);
}

static char	*check_absolute_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_path_dirs(char **path_dirs, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = create_full_path(path_dirs[i], cmd);
		if (full_path && access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		if (full_path)
			free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**path_dirs;
	char	*result;

	if (!cmd || !shell)
		return (NULL);
	result = check_absolute_path(cmd);
	if (result || ft_strchr(cmd, '/'))
		return (result);
	path_env = get_env_value(shell, "PATH");
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	result = search_in_path_dirs(path_dirs, cmd);
	free_argv(path_dirs);
	return (result);
}
