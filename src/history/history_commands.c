/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:52:38 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 15:21:55 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_history(void)
{
	int		hs_fd;
	char	*line;
	size_t	len;

	hs_fd = open(HISTORY_FILE, O_RDONLY);
	while (1)
	{
		line = get_next_line(hs_fd);
		if (!line)
			break ;
		len = ft_strlen(line);
		if (line [len - 1] == '\n')
			line[len - 1] = '\0';
		ft_printf("%s\n", line);
		free(line);
	}
	close(hs_fd);
}
