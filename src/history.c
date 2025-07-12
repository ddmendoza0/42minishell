/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:53:51 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/07/10 16:34:18 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*Inicializar el historial, crea el archivo .minishell_history y lo carga en memoria*/
int	initialize_history(void)
{
	int	hs_fd;
	char	*line;
	size_t	len;

	hs_fd = open(HISTORY_FILE, O_CREAT | O_RDWR , 0644);
	while (1)
	{
		line = get_next_line(hs_fd);
		if (!line)
			break ;
		len = ft_strlen(line);
		if (line [len - 1] == '\n')
			line[len - 1] = '\0';
		add_history(line);
		free(line);
	}
	close(hs_fd);
	return (open(HISTORY_FILE, O_RDWR | O_APPEND));
}

/*Escribir el historial en .minishell_history, falta añadir los timestamps*/
void	write_to_history_file(char *input, int history_fd)
{
	if (history_fd == -1)
	{
		perror("Error accessing the command history.\n");
		return;
	}
	write(history_fd, input, ft_strlen(input));
	write(history_fd, "\n", 1);
}


