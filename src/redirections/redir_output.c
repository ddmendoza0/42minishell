/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:01:10 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/17 13:01:21 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_output_file(t_redir_file *redir, t_shell *shell)
{
	int	flags;
	int	fd;

	if (redir->append_mode)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->expanded_path, flags, 0644);
	if (fd == -1)
	{
		print_error("minishell", redir->expanded_path, strerror(errno));
		shell->last_exit_status = 1;
		return (0);
	}
	redir->fd = fd;
	return (1);
}

int	validate_single_output(t_redir_file *redir, t_shell *shell)
{
	if (!redir->expanded_path || !*redir->expanded_path)
	{
		redir->fd = open(TMP_OUT_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (redir->fd == -1)
		{
			handle_system_error(shell, "output redirection (tmp)");
			return (0);
		}
		return (1);
	}
	return (open_output_file(redir, shell));
}
