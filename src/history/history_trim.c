/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_trim.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:05:53 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 15:06:20 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**read_history_lines(size_t *count)
{
	int		fd;
	char	**lines;
	char	*line;
	size_t	n;

	fd = open(HISTORY_FILE, O_RDONLY);
	if (fd == -1)
		return (NULL);
	lines = NULL;
	n = 0;
	line = get_next_line(fd);
	while (line)
	{
		lines = realloc(lines, sizeof(char *) * (n + 1)); //aqui falta validación del malloc
		lines[n] = line;
		n++;
		line = get_next_line(fd);
	}
	close(fd);
	*count = n;
	return (lines);
}

static void	free_history_lines(char **lines, size_t start, size_t end)
{
	size_t	i;

	i = start;
	while (i < end)
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}

static void	write_trimmed_history(char **lines, size_t count)
{
	int		fd;
	size_t	i;

	fd = open(HISTORY_FILE, O_WRONLY | O_TRUNC);
	i = 0;
	if (count > MAX_HISTORY)
		i = count - MAX_HISTORY;
	if (fd != -1)
	{
		while (i < count)
		{
			write(fd, lines[i], ft_strlen(lines[i]));
			write(fd, "\n", 1);
			free(lines[i]);
			i++;
		}
		close(fd);
	}
}

void	trim_history_file(void)
{
	char	**lines;
	size_t	count;

	lines = read_history_lines(&count);
	if (!lines)
		return ;
	if (count > MAX_HISTORY)
	{
		write_trimmed_history(lines, count);
		free_history_lines(lines, 0, count - MAX_HISTORY);
	}
	else
		free_history_lines(lines, 0, count);
}
