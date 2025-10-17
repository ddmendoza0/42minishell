/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:00:39 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/17 13:00:54 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_error(t_redir_file *redir, t_shell *shell,
		int saved_errno)
{
	if (saved_errno == ENOENT)
		print_error("minishell", redir->expanded_path,
			"No such file or directory");
	else if (saved_errno == EACCES)
		print_error("minishell", redir->expanded_path, "Permission denied");
	else
		print_error("minishell", redir->expanded_path, strerror(saved_errno));
	shell->last_exit_status = 1;
	return (0);
}

static int	validate_input_file(t_redir_file *redir, t_shell *shell)
{
	int	test_fd;
	int	saved_errno;

	test_fd = open(redir->expanded_path, O_RDONLY);
	if (test_fd == -1)
	{
		saved_errno = errno;
		return (handle_input_error(redir, shell, saved_errno));
	}
	redir->fd = test_fd;
	return (1);
}

int	validate_single_input(t_redir_file *redir, t_shell *shell)
{
	if (redir->is_heredoc)
	{
		if (!redir->expanded_path || !*redir->expanded_path)
			return (handle_syntax_error(shell, "newline"));
		return (1);
	}
	if (!redir->expanded_path || !*redir->expanded_path)
	{
		redir->fd = open(TMP_IN_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (redir->fd == -1)
		{
			handle_system_error(shell, "input redirection (tmp)");
			return (0);
		}
		return (1);
	}
	return (validate_input_file(redir, shell));
}
