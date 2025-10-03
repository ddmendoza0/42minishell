/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:01:49 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 17:01:57 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_interactive_signals(t_shell *shell)
{
	if (g_signal_received == SIGINT)
	{
		shell->last_exit_status = 130;
		g_signal_received = 0;
	}
}

int	get_signal_exit_status(int sig)
{
	return (128 + sig);
}
