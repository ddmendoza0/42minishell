/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 13:34:00 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/02 13:41:30 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_input(const char *input)
{
	if (!input)
		return (NULL);
	return (ft_strtrim(input, WHITESPACES));
}

void	cleanup_resources(t_command *cmd_tree,
		t_token *token_lst, char *clean_input)
{
	free_cmd_list(cmd_tree);
	free_token_lst(token_lst);
	free(clean_input);
}

void	execute_and_handle_signals(t_command *cmd_tree,
		t_shell *shell, char *input)
{
	if (strcmp(input, "history") == 0)
		cmd_history();
	setup_signals_execution();
	shell->last_exit_status = execute_command_tree(cmd_tree, shell);
	if (g_signal_received)
	{
		shell->last_exit_status = get_signal_exit_status(g_signal_received);
		g_signal_received = 0;
	}
}
