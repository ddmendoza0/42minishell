/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:11 by dmendoza          #+#    #+#             */
/*   Updated: 2025/09/30 15:15:04 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*trim_input(const char *input)
{
	if (!input)
		return (NULL);
	return (ft_strtrim(input, WHITESPACES));
}

static void	input_loop(int history_fd, t_shell *shell)
{
	char		*input;
	char		*clean_input;
	t_token		*token_lst;
	t_command	*cmd_tree;

	while (1)
	{
		setup_signals_interactive();
		input = readline("$minishell:");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		add_history(input);
		write_to_history_file(input, history_fd);
		handle_interactive_signals(shell);
		clean_input = trim_input(input);
		free(input);
		if (!clean_input || *clean_input == '\0')
		{
			free(clean_input);
			continue ;
		}
		if (!lexer(clean_input, &token_lst, shell))
		{
			printf("Error: Lexical analysis failed\n");
			free(clean_input);
			continue ;
		}
		cmd_tree = cmd_builder(&token_lst, shell);
		if (!cmd_tree)
		{
			printf("Error: Syntax analysis failed\n");
			free_token_lst(token_lst);
			free(clean_input);
			continue ;
		}
		if (!lexical_review(cmd_tree, shell))
		{
			printf("Error: Lexical review failed\n");
			free_cmd_list(cmd_tree);
			free_token_lst(token_lst);
			free(clean_input);
			continue ;
		}
		if (!validate_command_redirections(cmd_tree))
		{
			printf("Error: Redirection validation failed\n");
			free_cmd_list(cmd_tree);
			free_token_lst(token_lst);
			free(clean_input);
			continue ;
		}
		if (strcmp(input, "history") == 0)
			cmd_history();
		setup_signals_execution();
		shell->last_exit_status = execute_command_tree(cmd_tree, shell);
		if (g_signal_received)
		{
			shell->last_exit_status = get_signal_exit_status(g_signal_received);
			g_signal_received = 0;
		}
		free_cmd_list(cmd_tree);
		free_token_lst(token_lst);
		free(clean_input);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int		history_fd;
	t_shell	shell;

	(void)argc;
	(void)argv;
	if (!init_shell(&shell, envp))
	{
		fprintf(stderr, "Error: Failed to initialize shell\n");
		return (EXIT_FAILURE);
	}
	history_fd = initialize_history();
	if (history_fd == -1)
	{
		fprintf(stderr, "Warning: Could not initialize history\n");
	}
	setup_signals_interactive();
	input_loop(history_fd, &shell);
	if (history_fd >= 0)
		close(history_fd);
	rl_clear_history();
	cleanup_shell(&shell);
	return (shell.last_exit_status);
}
