/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:53:51 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/02 12:25:44 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	initialize_history(void)
{
	int		hs_fd;
	//char	*line;
	//size_t	len;

	hs_fd = open(HISTORY_FILE, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (hs_fd == -1)
	{
		perror("Error opening history file");
		return (-1);
	}/*
	while (1)
	{
		line = get_next_line(hs_fd);
		if (!line)
			break ;
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		add_history(line);
		free(line);
	}
	close(hs_fd);
	while (history_length > MAX_HISTORY)
		remove_history(0);*/
	close(hs_fd);
	return (open(HISTORY_FILE, O_RDWR | O_APPEND));
}

void	write_to_history_file(char *input, int history_fd)
{
	if (history_fd == -1)
	{
		perror("Error accessing the command history.\n");
		return ;
	}
	write(history_fd, input, ft_strlen(input));
	write(history_fd, "\n", 1);
	trim_history_file();
}
