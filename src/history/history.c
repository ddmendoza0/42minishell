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

//check retorno de -1 si falla
int	initialize_history(void)
{
	int		hs_fd;
	char	*line;
	size_t	len;

	hs_fd = open(HISTORY_FILE, O_CREAT | O_RDWR, 0644);
	if (hs_fd == -1)
	{
		perror("Error opening history file");
		return (-1);
	}
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
		remove_history(0);
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
/*
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
		lines = realloc(lines, sizeof(char *) * (n + 1));
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
// Trims the history file to the maximum allowed history size.
void	trim_history_file(void)
{
	int		fd;
	char	**lines;
	size_t	count;
	size_t	i;
	char	*line;

	count = 0;
	i = 0;
	lines = NULL;
	fd = open(HISTORY_FILE, O_RDONLY);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
	while (line)
	{
		lines = realloc(lines, sizeof(char *) * (count + 1));
		lines[count++] = line;
		line = get_next_line(fd);
	}
	close(fd);
	if (count > MAX_HISTORY)
	{
		fd = open(HISTORY_FILE, O_WRONLY | O_TRUNC);
		if (fd == -1)
		{
			i = 0;
			while (i < count)
				free(lines[i++]);
			free(lines);
			return ;
		}
		i = count - MAX_HISTORY;
		while (i < count)
		{
			write(fd, lines[i], ft_strlen(lines[i]));
			write(fd, "\n", 1);
			free(lines[i]);
			i++;
		}
		close(fd);
		i = 0;
		while (i < count - MAX_HISTORY)
			free(lines[i++]);
	}
	else
	{
		i = 0;
		while (i < count)
			free(lines[i++]);
	}
	free(lines);
}*/
