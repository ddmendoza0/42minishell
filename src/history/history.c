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

	hs_fd = open(HISTORY_FILE, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (hs_fd == -1)
	{
		perror("Error opening history file");
		return (-1);
	}
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
