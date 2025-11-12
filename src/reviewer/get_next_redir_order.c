/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_redir_order.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:10:08 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/11/12 17:10:18 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_redir_order(t_command *cmd)
{
	t_redir_list	*in;
	t_redir_list	*out;
	int				max_order;

	max_order = 0;
	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->order > max_order)
			max_order = in->redir->order;
		in = in->next;
	}
	out = cmd->output_redirs;
	while (out)
	{
		if (out->redir->order > max_order)
			max_order = out->redir->order;
		out = out->next;
	}
	return (max_order + 1);
}
