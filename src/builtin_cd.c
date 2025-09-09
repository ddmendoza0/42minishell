/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:25:23 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:25:27 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_pwd_var(t_shell *shell, char *new_pwd)
{
	char	*pwd_var;

	if (!shell || !new_pwd)
		return (0);
	pwd_var = ft_strjoin("PWD=", new_pwd);
	if (!pwd_var)
		return (0);
	if (!set_env_var(shell, pwd_var))
	{
		free(pwd_var);
		return (0);
	}
	free(pwd_var);
	if (shell->cwd)
		free(shell->cwd);
	shell->cwd = ft_strdup(new_pwd);
	return (1);
}

static char	*get_current_directory(void)
{
	return (getcwd(NULL, 0));
}

static int	validate_cd_args(char **argv, t_shell *shell)
{
	if (!argv[1])
	{
		print_error("minishell: cd", NULL, "missing argument");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	if (argv[2])
	{
		print_error("minishell: cd", NULL, "too many arguments");
		return (set_exit_status(shell, EXIT_FAILURE));
	}
	return (0);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*new_pwd;
	int		validation_result;

	if (!shell)
		return (EXIT_FAILURE);
	validation_result = validate_cd_args(argv, shell);
	if (validation_result != 0)
		return (validation_result);
	if (chdir(argv[1]) != 0)
		return (cd_error(shell, argv[1], errno));
	new_pwd = get_current_directory();
	if (!new_pwd)
		return (handle_error(shell, ERR_MEMORY, "getting current directory"));
	if (!update_pwd_var(shell, new_pwd))
	{
		free(new_pwd);
		return (handle_error(shell, ERR_MEMORY, "updating PWD variable"));
	}
	free(new_pwd);
	return (set_exit_status(shell, EXIT_SUCCESS));
}
