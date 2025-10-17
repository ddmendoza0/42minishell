/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:02:16 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/17 13:02:35 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_command_redirections(t_command *cmd, t_shell *shell)
{
	t_redir_file	*current;
	int				order;
	int				result;

	order = -1;
	while (1)
	{
		current = get_next_redir_by_order(cmd, order);
		if (!current)
			break ;
		if (check_if_input(current, cmd))
			result = validate_single_input(current, shell);
		else
			result = validate_single_output(current, shell);
		if (!result)
		{
			close_previous_redirs(cmd, current->order);
			return (0);
		}
		order = current->order;
	}
	return (process_heredocs(cmd, shell));
}
