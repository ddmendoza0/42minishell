/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_checkers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:53:52 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:53:54 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_heredoc_mode(t_command *cmd)
{
	if (!cmd || !cmd->input_redir)
		return (0);
	return (cmd->input_redir->is_heredoc);
}
