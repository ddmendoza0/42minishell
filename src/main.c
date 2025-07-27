/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:11 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/13 17:35:06 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*trim_input(const char *input)
{
	if (!input)
		return (NULL);
	return (ft_strtrim(input, WHITESPACES));
}

static void input_loop(int history_fd, t_shell* shell)
{
	char	*input;
	char	*clean_input;
	t_token *token_lst;

	while (1)
	{
		input = readline("$minishell:");
		//VALIDATIONS
		if (input == NULL)//Ctrl+D
			break;
		clean_input = trim_input(input);
		if (!clean_input || *clean_input == '\0')//this skips empty line
		{
			free(input);
			free(clean_input);
			continue;
		}
		if(strcmp(clean_input, "exit") == 0)
		{
			//working history file logic
			//write_history(HISTORY_FILE);
			free(clean_input);
			free(input);
			break;
		}
		//MAIN LOGIC
		if (!lexer(clean_input, &token_lst, shell))
		{
			// Lexer failed - print error and continue
			free(clean_input);
			free(input);
			continue; // Return to prompt
		}
		add_history(input);
		write_to_history_file(input, history_fd);
		if (strcmp(input, "history") == 0)
			cmd_history();
		//valid command checker here
		//execute command, else throw error
		ft_printf("%s\n", clean_input);//test placeholder
		free_token_lst(token_lst);
		free(clean_input);
		free(input);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int	history_fd;
	t_shell shell;

	//casting !CLEANUP!
	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	history_fd = initialize_history();
	input_loop(history_fd, &shell);
	close(history_fd);
	rl_clear_history();
	// Cleanup shell state
	cleanup_shell(&shell);
	// Return appropriate exit code
	return (shell.last_exit_status);
}
