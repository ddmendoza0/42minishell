/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:53:20 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 19:49:56 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_file	*create_redir_file(t_token *org, int append_mode, int is_hdoc)
{
	t_redir_file	*redir;

	if (!org)
		return (NULL);
	redir = malloc(sizeof(t_redir_file));
	if (!redir)
		return (NULL);
	redir->original_token = org;
	redir->expanded_path = NULL;
	redir->append_mode = append_mode;
	redir->is_heredoc = is_hdoc;
	redir->fd = -1;
	redir->order = 0;
	return (redir);
}

void	free_redir_file(t_redir_file *redir)
{
	size_t	path_len;

	if (!redir)
		return ;
	if (redir->fd >= 0)
		close(redir->fd);
	if (redir->expanded_path)
	{
		path_len = ft_strlen(redir->expanded_path);
		if ((ft_strncmp(redir->expanded_path, TMP_IN_FILE, path_len + 1) == 0)
			|| (ft_strncmp(redir->expanded_path, TMP_OUT_FILE,
					path_len + 1) == 0) || (ft_strncmp(redir->expanded_path,
					TMP_HEREDOC_FILE, path_len + 1) == 0))
		{
			unlink(redir->expanded_path);
		}
		free(redir->expanded_path);
	}
	free(redir);
}
