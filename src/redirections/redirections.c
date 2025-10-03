/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:23:20 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/03 18:50:03 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
#include "minishell.h"

int	handle_heredoc(t_redir_file *redir)
{
	int		fd;
	char	*line;
	size_t	delimiter_len;

	fd = open(TMP_HEREDOC_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc (tmp)");
		return (0);
	}
	delimiter_len = ft_strlen(redir->expanded_path);
	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = readline(NULL);
		if (!line)
			break ;
		if (ft_strncmp(line, redir->expanded_path, delimiter_len) == 0
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	redir->fd = fd;
	return (1);
}

int	validate_input_redirection(t_redir_file *input_redir)
{
	if (input_redir)
	{
		if (input_redir->is_heredoc)
		{
			if (!input_redir->expanded_path || !*input_redir->expanded_path)
			{
				print_error("minishell", "syntax error",
					"unexpected token `newline'");
				return (0);
			}
			if (!handle_heredoc(input_redir))
				return (0);
		}
		else if (!input_redir->expanded_path || !*input_redir->expanded_path)
		{
			input_redir->fd
				= open(TMP_IN_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
			if (input_redir->fd == -1)
			{
				perror("minishell: input redirection (tmp)");
				return (0);
			}
		}
		else
		{
			input_redir->fd = open(input_redir->expanded_path, O_RDONLY);
			if (input_redir->fd == -1)
			{
				print_error("minishell", input_redir
					->expanded_path, strerror(errno));
				return (0);
			}
		}
	}
	return (1);
}

int	validate_output_redirection(t_redir_file *output_redir)
{
	if (output_redir)
	{
		if (!output_redir->expanded_path || !*output_redir->expanded_path)
		{
			output_redir->fd
				= open(TMP_OUT_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
			if (output_redir->fd == -1)
			{
				perror("minishell: output redirection (tmp)");
				return (0);
			}
		}
		else
		{
			if (output_redir->append_mode)
				output_redir->fd = open(output_redir
						->expanded_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				output_redir->fd = open(output_redir
						->expanded_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (output_redir->fd == -1)
			{
				print_error("minishell", output_redir
					->expanded_path, strerror(errno));
				return (0);
			}
		}
	}
	return (1);
}

int	validate_command_redirections(t_command *cmd)
{
	if (!validate_input_redirection(cmd->input_redir))
		return (0);
	if (!validate_output_redirection(cmd->output_redir))
		return (0);
	if (cmd->subshell && !validate_command_redirections(cmd->subshell))
		return (0);
	if (cmd->next && !validate_command_redirections(cmd->next))
		return (0);
	return (1);
}*/

#include "minishell.h"
/*
int	handle_heredoc(t_redir_file *redir)
{
	int		fd;
	char	*line;
	size_t	delimiter_len;

	fd = open(TMP_HEREDOC_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		handle_system_error(NULL, "heredoc (tmp)");
		return (0);
	}
	delimiter_len = ft_strlen(redir->expanded_path);
	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = readline(NULL);
		if (!line)
			break ;
		if (ft_strncmp(line, redir->expanded_path, delimiter_len) == 0
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	redir->fd = fd;
	return (1);
}

int	validate_input_redirection(t_redir_file *input_redir, t_shell *shell)
{
	int	saved_errno;

	if (input_redir)
	{
		if (input_redir->is_heredoc)
		{
			if (!input_redir->expanded_path || !*input_redir->expanded_path)
				return (handle_syntax_error(shell, "newline"));
			if (!handle_heredoc(input_redir))
				return (0);
		}
		else if (!input_redir->expanded_path || !*input_redir->expanded_path)
		{
			input_redir->fd
				= open(TMP_IN_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
			if (input_redir->fd == -1)
			{
				handle_system_error(shell, "input redirection (tmp)");
				return (0);
			}
		}
		else
		{
			input_redir->fd = open(input_redir->expanded_path, O_RDONLY);
			if (input_redir->fd == -1)
			{
				saved_errno = errno;  // GUARDA errno AQUÍ
            if (saved_errno == ENOENT)
                handle_error(shell, ERR_NO_SUCH_FILE, input_redir->expanded_path);
            else if (saved_errno == EACCES)
                handle_error(shell, ERR_PERMISSION_DENIED, input_redir->expanded_path);
            else
                print_error("minishell", input_redir->expanded_path, strerror(saved_errno));
            return (0);
			}
		}
	}
	return (1);
}
int	validate_input_redirection(t_redir_file *input_redir, t_shell *shell)
{
	int	saved_errno;

	if (!input_redir)
		return (1);
	if (input_redir->is_heredoc)
	{
		if (!input_redir->expanded_path || !*input_redir->expanded_path)
			return (handle_syntax_error(shell, "newline"));
		if (!handle_heredoc(input_redir))
			return (0);
	}
	else if (!input_redir->expanded_path || !*input_redir->expanded_path)
	{
		input_redir->fd = open(TMP_IN_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (input_redir->fd == -1)
		{
			handle_system_error(shell, "input redirection (tmp)");
			return (0);
		}
	}
	else
	{
		input_redir->fd = open(input_redir->expanded_path, O_RDONLY);
		if (input_redir->fd == -1)
		{
			saved_errno = errno;
			if (saved_errno == ENOENT)
				print_error("minishell", input_redir->expanded_path,
					"No such file or directory");
			else if (saved_errno == EACCES)
				print_error("minishell", input_redir->expanded_path,
					"Permission denied");
			else
				print_error("minishell", input_redir->expanded_path,
					strerror(saved_errno));
			shell->last_exit_status = 1;
			return (0);
		}
	}
	return (1);
}

int	validate_output_redirection(t_redir_file *output_redir, t_shell *shell)
{
	int	saved_errno;

	if (output_redir)
	{
		if (!output_redir->expanded_path || !*output_redir->expanded_path)
		{
			output_redir->fd
				= open(TMP_OUT_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
			if (output_redir->fd == -1)
				return (handle_syntax_error(shell, "newline"));
		}
		else
		{
			if (output_redir->append_mode)
				output_redir->fd = open(output_redir
						->expanded_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				output_redir->fd = open(output_redir
						->expanded_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (output_redir->fd == -1)
			{
							saved_errno = errno;
			if (saved_errno == EACCES)
				handle_error(shell, ERR_PERMISSION_DENIED,
					output_redir->expanded_path);
			else if (saved_errno == EISDIR)
				handle_error(shell, ERR_IS_DIRECTORY,
					output_redir->expanded_path);
			else
				print_error("minishell", output_redir->expanded_path,
					strerror(saved_errno));
			return (0);
			}
		}
	}
	return (1);
}

int	validate_command_redirections(t_command *cmd, t_shell *shell)
{
	if (!validate_input_redirection(cmd->input_redir, shell))
		return (0);
	if (!validate_output_redirection(cmd->output_redir, shell))
		return (0);
	return (1);
}*/
/*int	handle_heredoc(t_redir_file *redir)
{
	int		fd;
	char	*line;
	size_t	delimiter_len;

	fd = open(TMP_HEREDOC_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		handle_system_error(NULL, "heredoc (tmp)");
		return (0);
	}
	delimiter_len = ft_strlen(redir->expanded_path);
	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = readline(NULL);
		if (!line)
			break ;
		if (ft_strncmp(line, redir->expanded_path, delimiter_len) == 0
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	redir->fd = fd;
	return (1);
}

static int	validate_all_input_redirs(t_redir_list *list, t_shell *shell)
{
	t_redir_list	*current;
	int				test_fd;
	int				saved_errno;

	current = list;
	while (current)
	{
		if (current->redir->is_heredoc)
		{
			if (!current->redir->expanded_path
				|| !*current->redir->expanded_path)
				return (handle_syntax_error(shell, "newline"));
		}
		else if (current->redir->expanded_path
			&& *current->redir->expanded_path)
		{
			test_fd = open(current->redir->expanded_path, O_RDONLY);
			if (test_fd == -1)
			{
				saved_errno = errno;
				if (saved_errno == ENOENT)
					print_error("minishell", current->redir->expanded_path,
						"No such file or directory");
				else if (saved_errno == EACCES)
					print_error("minishell", current->redir->expanded_path,
						"Permission denied");
				else
					print_error("minishell", current->redir->expanded_path,
						strerror(saved_errno));
				shell->last_exit_status = 1;
				return (0);
			}
			close(test_fd);
		}
		current = current->next;
	}
	return (1);
}

static int	setup_last_valid_input(t_redir_list *list, t_shell *shell)
{
	t_redir_file	*last_redir;

	last_redir = get_last_redir(list);
	if (!last_redir)
		return (1);
	if (last_redir->is_heredoc)
	{
		if (!handle_heredoc(last_redir))
			return (0);
	}
	else if (!last_redir->expanded_path || !*last_redir->expanded_path)
	{
		last_redir->fd = open(TMP_IN_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (last_redir->fd == -1)
		{
			handle_system_error(shell, "input redirection (tmp)");
			return (0);
		}
	}
	else
	{
		last_redir->fd = open(last_redir->expanded_path, O_RDONLY);
		if (last_redir->fd == -1)
		{
			handle_system_error(shell, last_redir->expanded_path);
			return (0);
		}
	}
	return (1);
}

int	validate_input_redirection(t_redir_list *input_redirs, t_shell *shell)
{
	if (!input_redirs)
		return (1);
	if (!validate_all_input_redirs(input_redirs, shell))
		return (0);
	return (setup_last_valid_input(input_redirs, shell));
}*/

/*int	validate_output_redirection(t_redir_list *output_redirs, t_shell *shell)
{
	t_redir_file	*last_redir;
	int				flags;

	if (!output_redirs)
		return (1);
	last_redir = get_last_redir(output_redirs);
	if (!last_redir)
		return (1);
	if (!last_redir->expanded_path || !*last_redir->expanded_path)
	{
		last_redir->fd = open(TMP_OUT_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (last_redir->fd == -1)
		{
			handle_system_error(shell, "output redirection (tmp)");
			return (0);
		}
	}
	else
	{
		if (last_redir->append_mode)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		last_redir->fd = open(last_redir->expanded_path, flags, 0644);
		if (last_redir->fd == -1)
		{
			handle_system_error(shell, last_redir->expanded_path);
			return (0);
		}
	}
	return (1);
}*/
/*static int	open_all_output_redirs(t_redir_list *list, t_shell *shell)
{
	t_redir_list	*current;
	int				flags;
	int				fd;

	current = list;
	while (current)
	{
		if (!current->redir->expanded_path
			|| !*current->redir->expanded_path)
		{
			current = current->next;
			continue ;
		}
		if (current->redir->append_mode)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(current->redir->expanded_path, flags, 0644);
		if (fd == -1)
		{
			print_error("minishell", current->redir->expanded_path,
				strerror(errno));
			shell->last_exit_status = 1;
			return (0);
		}
		if (!current->next)
			current->redir->fd = fd;
		else
			close(fd);
		current = current->next;
	}
	return (1);
}

int	validate_output_redirection(t_redir_list *output_redirs, t_shell *shell)
{
	t_redir_file	*last_redir;

	if (!output_redirs)
		return (1);
	if (!open_all_output_redirs(output_redirs, shell))
		return (0);
	last_redir = get_last_redir(output_redirs);
	if (!last_redir)
		return (1);
	if (!last_redir->expanded_path || !*last_redir->expanded_path)
	{
		last_redir->fd = open(TMP_OUT_FILE,
				O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (last_redir->fd == -1)
		{
			handle_system_error(shell, "output redirection (tmp)");
			return (0);
		}
	}
	return (1);
}

int	validate_command_redirections(t_command *cmd, t_shell *shell)
{
	if (!validate_output_redirection(cmd->output_redirs, shell))
		return (0);
	if (!validate_input_redirection(cmd->input_redirs, shell))
		{
		// Si falla input, cierra los fds de output que ya abrimos
		t_redir_file *output = get_last_redir(cmd->output_redirs);
		if (output && output->fd >= 0)
		{
			close(output->fd);
			output->fd = -1;
		}
		return (0);
	}
	return (1);
}*/
int	handle_heredoc(t_redir_file *redir)
{
	int		fd;
	char	*line;
	size_t	delimiter_len;

	fd = open(TMP_HEREDOC_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fd == -1)
	{
		handle_system_error(NULL, "heredoc (tmp)");
		return (0);
	}
	delimiter_len = ft_strlen(redir->expanded_path);
	while (1)
	{
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		line = readline(NULL);
		if (!line)
			break ;
		if (ft_strncmp(line, redir->expanded_path, delimiter_len) == 0
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	lseek(fd, 0, SEEK_SET);
	redir->fd = fd;
	return (1);
}
static int	process_heredocs(t_command *cmd, t_shell *shell)
{
	t_redir_list	*in;
	t_redir_file	*last_heredoc;

	(void)shell;
	last_heredoc = NULL;
	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->is_heredoc)
		{
			if (!last_heredoc || in->redir->order > last_heredoc->order)
				last_heredoc = in->redir;
		}
		in = in->next;
	}
	if (last_heredoc)
	{
		if (!handle_heredoc(last_heredoc))
			return (0);
	}
	return (1);
}

static t_redir_file	*get_next_redir_by_order(t_command *cmd, int current_order)
{
	t_redir_list	*in;
	t_redir_list	*out;
	t_redir_file	*next;
	int				min_order;

	next = NULL;
	min_order = 9999;
	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->order > current_order && in->redir->order < min_order)
		{
			min_order = in->redir->order;
			next = in->redir;
		}
		in = in->next;
	}
	out = cmd->output_redirs;
	while (out)
	{
		if (out->redir->order > current_order && out->redir->order < min_order)
		{
			min_order = out->redir->order;
			next = out->redir;
		}
		out = out->next;
	}
	return (next);
}

/*static int	is_input_redir(t_redir_file *redir)
{
	t_redir_list	*dummy_list;

	dummy_list = NULL;
	// Si no tiene append_mode y puede ser input
	return (!redir->append_mode);
}*/

static int	validate_single_input(t_redir_file *redir, t_shell *shell)
{
	int	test_fd;
	int	saved_errno;

	if (redir->is_heredoc)
	{
		if (!redir->expanded_path || !*redir->expanded_path)
			return (handle_syntax_error(shell, "newline"));
		// El heredoc se procesa después
		return (1);
	}
	if (!redir->expanded_path || !*redir->expanded_path)
	{
		redir->fd = open(TMP_IN_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (redir->fd == -1)
		{
			handle_system_error(shell, "input redirection (tmp)");
			return (0);
		}
		return (1);
	}
	test_fd = open(redir->expanded_path, O_RDONLY);
	if (test_fd == -1)
	{
		saved_errno = errno;
		if (saved_errno == ENOENT)
			print_error("minishell", redir->expanded_path,
				"No such file or directory");
		else if (saved_errno == EACCES)
			print_error("minishell", redir->expanded_path, "Permission denied");
		else
			print_error("minishell", redir->expanded_path, strerror(saved_errno));
		shell->last_exit_status = 1;
		return (0);
	}
	redir->fd = test_fd;
	return (1);
}

static int	validate_single_output(t_redir_file *redir, t_shell *shell)
{
	int	flags;
	int	fd;

	if (!redir->expanded_path || !*redir->expanded_path)
	{
		redir->fd = open(TMP_OUT_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
		if (redir->fd == -1)
		{
			handle_system_error(shell, "output redirection (tmp)");
			return (0);
		}
		return (1);
	}
	if (redir->append_mode)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->expanded_path, flags, 0644);
	if (fd == -1)
	{
		print_error("minishell", redir->expanded_path, strerror(errno));
		shell->last_exit_status = 1;
		return (0);
	}
	redir->fd = fd;
	return (1);
}

static void	close_previous_redirs(t_command *cmd, int up_to_order)
{
	t_redir_list	*in;
	t_redir_list	*out;

	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir->order < up_to_order && in->redir->fd >= 0)
		{
			close(in->redir->fd);
			in->redir->fd = -1;
		}
		in = in->next;
	}
	out = cmd->output_redirs;
	while (out)
	{
		if (out->redir->order < up_to_order && out->redir->fd >= 0)
		{
			close(out->redir->fd);
			out->redir->fd = -1;
		}
		out = out->next;
	}
}

static int	check_if_input(t_redir_file *redir, t_command *cmd)
{
	t_redir_list	*in;

	in = cmd->input_redirs;
	while (in)
	{
		if (in->redir == redir)
			return (1);
		in = in->next;
	}
	return (0);
}

int	validate_command_redirections(t_command *cmd, t_shell *shell)
{
	t_redir_file	*current;
	int				order;
	int				result;

	order = -1;
	while (1)
	{
		current = get_next_redir_by_order(cmd, order);
		if (!current)
			break ;
		if (check_if_input(current, cmd))
			result = validate_single_input(current, shell);
		else
			result = validate_single_output(current, shell);
		if (!result)
		{
			close_previous_redirs(cmd, current->order);
			return (0);
		}
		order = current->order;
	}
	return (process_heredocs(cmd, shell));
}

