/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 13:26:40 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/03 18:19:18 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_and_prepare_input(int history_fd,
		t_shell *shell, char **clean_input)
{
	char	*input;

	setup_signals_interactive();
	input = readline("$minishell:");
	if (input == NULL)
	{
		printf("exit\n");
		return (-1);
	}
	add_history(input);
	write_to_history_file(input, history_fd);
	handle_interactive_signals(shell);
	*clean_input = trim_input(input);
	free(input);
	if (!*clean_input || **clean_input == '\0')
	{
		free(*clean_input);
		return (0);
	}
	return (1);
}

static int	lex_and_parse_input(char *clean_input,
		t_token **token_lst, t_command **cmd_tree, t_shell *shell)
{
	if (!lexer(clean_input, token_lst, shell))
	{
		printf("Error: Lexical analysis failed\n");
		return (0);
	}
	*cmd_tree = cmd_builder(token_lst, shell);
	if (!*cmd_tree)
	{
		printf("Error: Syntax analysis failed\n");
		free_token_lst(*token_lst);
		return (0);
	}
	return (1);
}

static int	validate_and_review_command(t_command *cmd_tree,
		t_token *token_lst, char *clean_input, t_shell *shell)
{
	if (!lexical_review(cmd_tree, shell))
	{
		printf("Error: Lexical review failed\n");
		free_cmd_list(cmd_tree);
		free_token_lst(token_lst);
		free(clean_input);
		return (0);
	}
	if (!has_pipeline(cmd_tree))
	{
		if (!validate_command_redirections(cmd_tree, shell))
		{
			free_cmd_list(cmd_tree);
			free_token_lst(token_lst);
			free(clean_input);
			return (0);
		}
	}
	return (1);
}

static void	input_loop(int history_fd, t_shell *shell)
{
	char		*clean_input;
	t_token		*token_lst;
	t_command	*cmd_tree;
	int			prep_result;

	while (1)
	{
		prep_result = get_and_prepare_input(history_fd, shell, &clean_input);
		if (prep_result == -1)
			break ;
		if (prep_result == 0)
			continue ;
		token_lst = NULL;
		cmd_tree = NULL;
		if (!lex_and_parse_input(clean_input, &token_lst, &cmd_tree, shell))
		{
			free(clean_input);
			continue ;
		}
		if (!validate_and_review_command(cmd_tree,
				token_lst, clean_input, shell))
			continue ;
		execute_and_handle_signals(cmd_tree, shell, clean_input);
		cleanup_resources(cmd_tree, token_lst, clean_input);
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
