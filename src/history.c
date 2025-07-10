/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:53:51 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/07/10 13:36:37 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
