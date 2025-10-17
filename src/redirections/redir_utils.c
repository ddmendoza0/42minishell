/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:01:48 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/17 13:02:00 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_file	*check_input_redirs(t_command *cmd, int current_order,
		int *min_order)
{
	t_redir_list	*in;
	t_redir_file	*next;

	next = NULL;
	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->order > current_order && in->redir->order < *min_order)
		{
			*min_order = in->redir->order;
			next = in->redir;
		}
		in = in->next;
	}
	return (next);
}

t_redir_file	*get_next_redir_by_order(t_command *cmd, int current_order)
{
	t_redir_list	*out;
	t_redir_file	*next;
	int				min_order;

	min_order = 9999;
	next = check_input_redirs(cmd, current_order, &min_order);
	out = cmd->output_redirs;
	while (out)
	{
		if (out->redir->order > current_order && out->redir->order < min_order)
		{
			min_order = out->redir->order;
			next = out->redir;
		}
		out = out->next;
	}
	return (next);
}

void	close_previous_redirs(t_command *cmd, int up_to_order)
{
	t_redir_list	*in;
	t_redir_list	*out;

	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->order < up_to_order && in->redir->fd >= 0)
		{
			close(in->redir->fd);
			in->redir->fd = -1;
		}
		in = in->next;
	}
	out = cmd->output_redirs;
	while (out)
	{
		if (out->redir->order < up_to_order && out->redir->fd >= 0)
		{
			close(out->redir->fd);
			out->redir->fd = -1;
		}
		out = out->next;
	}
}

int	check_if_input(t_redir_file *redir, t_command *cmd)
{
	t_redir_list	*in;

	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir == redir)
			return (1);
		in = in->next;
	}
	return (0);
}
