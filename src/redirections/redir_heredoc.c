/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:00:09 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/17 13:00:23 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_heredoc_lines(int fd, t_redir_file *redir)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(redir->expanded_path);
	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = readline(NULL);
		if (!line)
			break ;
		if (ft_strncmp(line, redir->expanded_path, delimiter_len) == 0
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (1);
}

int	handle_heredoc(t_redir_file *redir)
{
	int		fd;

	fd = open(TMP_HEREDOC_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		handle_system_error(NULL, "heredoc (tmp)");
		return (0);
	}
	read_heredoc_lines(fd, redir);
	lseek(fd, 0, SEEK_SET);
	redir->fd = fd;
	return (1);
}

int	process_heredocs(t_command *cmd, t_shell *shell)
{
	t_redir_list	*in;
	t_redir_file	*last_heredoc;

	(void)shell;
	last_heredoc = NULL;
	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->is_heredoc)
		{
			if (!last_heredoc || in->redir->order > last_heredoc->order)
				last_heredoc = in->redir;
		}
		in = in->next;
	}
	if (last_heredoc)
	{
		if (!handle_heredoc(last_heredoc))
			return (0);
	}
	return (1);
}
