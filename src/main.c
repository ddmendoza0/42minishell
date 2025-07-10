/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:11 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/10 17:00:53 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void input_loop(int history_fd)
{
	char *input;

	while (1)
	{
		input = readline("$minishell:");
		
		if (input == NULL)//Ctrl+D
			break;
		if(strcmp(input, "exit") == 0)
		{
			//working history file logic
			//write_history(HISTORY_FILE);
			free(input);
			break;
		}
		if (*input != '\0')//dont save empty
		{
			add_history(input);
			write_to_history_file(input, history_fd);
			if (strcmp(input, "history") == 0)
				cmd_history();
			//valid command checker here
			//execute command, else throw error
		}
		//logic

		ft_printf("%s\n", input);//test placeholder
		free(input);
	}
}



int	main(int argc, char *argv[])
{
	int	history_fd;
	//casting !CLEANUP!
	(void)argc;
	(void)argv;	

	history_fd = initialize_history();
	input_loop(history_fd);
	close(history_fd);
	rl_clear_history();
	return (0);
}
