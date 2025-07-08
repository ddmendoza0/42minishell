/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:11 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/08 19:19:34 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void input_loop(void)
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
			add_history(input);
		
		//logic

		ft_printf("%s\n", input);//test placeholder
		free(input);
	}
}



int	main(int argc, char *argv[])
{
	//casting !CLEANUP!
	(void)argc;
	(void)argv;	

	input_loop();
	return (0);
}
