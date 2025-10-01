/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_executioner.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:27:37 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:27:39 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

/*
 * UTILITY FUNCTIONS
 */
void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

/*
 * BUILTIN COMMAND DETECTION
 */
static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (1);
	return (0);
}

/*
 * BUILTIN COMMAND DISPATCHER
 */
static int	execute_builtin(char **argv, t_shell *shell)
{
	char	*cmd;

	if (!argv || !argv[0] || !shell)
		return (EXIT_FAILURE);
	cmd = argv[0];
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (builtin_cd(argv, shell));
	else if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (builtin_pwd(argv, shell));
	else if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (builtin_echo(argv, shell));
	else if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (builtin_export(argv, shell));
	else if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (builtin_unset(argv, shell));
	else if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (builtin_env(argv, shell));
	else if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (builtin_exit(argv, shell));
	return (EXIT_FAILURE);
}

/*
 * PATH RESOLUTION HELPERS
 */
static char	*create_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*result;

	if (!dir || !cmd)
		return (NULL);
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, cmd);
	free(temp);
	return (result);
}

static char	*check_absolute_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static char	*search_in_path_dirs(char **path_dirs, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = create_full_path(path_dirs[i], cmd);
		if (full_path && access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		if (full_path)
			free(full_path);
		i++;
	}
	return (NULL);
}

static char	*find_executable(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**path_dirs;
	char	*result;

	if (!cmd || !shell)
		return (NULL);
	result = check_absolute_path(cmd);
	if (result || ft_strchr(cmd, '/'))
		return (result);
	path_env = get_env_value(shell, "PATH");
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	result = search_in_path_dirs(path_dirs, cmd);
	free_argv(path_dirs);
	return (result);
}

static void	execute_child_process(char *exe_path, char **argv, t_shell *shell)
{
	setup_signals_default();
	execve(exe_path, argv, shell->env);
	handle_system_error(shell, argv[0]);
	exit(EXIT_CANNOT_EXECUTE);
}

static int	handle_child_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit: 3\n", 8);
		return (128 + sig);
	}
	return (EXIT_FAILURE);
}

/*
 * EXTERNAL COMMAND EXECUTION
 */
static int	execute_external(char **argv, t_shell *shell)
{
	char	*executable_path;
	pid_t	pid;
	int		status;

	executable_path = find_executable(argv[0], shell);
	if (!executable_path)
		return (handle_error(shell, ERR_COMMAND_NOT_FOUND, argv[0]));
	pid = fork();
	if (pid == -1)
	{
		free(executable_path);
		return (handle_system_error(shell, "fork"));
	}
	else if (pid == 0)
		execute_child_process(executable_path, argv, shell);
	free(executable_path);
	waitpid(pid, &status, 0);
	return (handle_child_status(status));
}

/*
 * REDIRECTION SETUP FUNCTIONS - CORREGIDO
 */
static int	setup_input_redirection(t_redir_file *input_redir, t_shell *shell)
{
	int	fd;

	if (!input_redir)
		return (1);
	if (input_redir->is_heredoc && input_redir->fd >= 0)
		fd = input_redir->fd;
	else
		fd = open(input_redir->expanded_path, O_RDONLY);
	if (fd == -1)
	{
		handle_system_error(shell, input_redir->expanded_path);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		handle_system_error(shell, "dup2");
		return (0);
	}
	close(fd);
	if (input_redir->is_heredoc)
		input_redir->fd = -1;
	return (1);
}

static int	setup_output_redirection(t_redir_file *output_redir, t_shell *shell)
{
	int	fd;
	int	flags;

	if (!output_redir)
		return (1);
	flags = O_WRONLY | O_CREAT;
	if (output_redir->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(output_redir->expanded_path, flags, 0644);
	if (fd == -1)
	{
		handle_system_error(shell, output_redir->expanded_path);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		handle_system_error(shell, "dup2");
		return (0);
	}
	close(fd);
	return (1);
}

static void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		if (dup2(saved_stdin, STDIN_FILENO) == -1)
			perror("restore stdin");
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		if (dup2(saved_stdout, STDOUT_FILENO) == -1)
			perror("restore stdout");
		close(saved_stdout);
	}
}

static int	save_standard_fds(int *s_stdin, int *s_stdout, t_shell *shell)
{
	*s_stdin = dup(STDIN_FILENO);
	*s_stdout = dup(STDOUT_FILENO);
	if (*s_stdin == -1 || *s_stdout == -1)
	{
		if (*s_stdin != -1)
			close(*s_stdin);
		if (*s_stdout != -1)
			close(*s_stdout);
		handle_system_error(shell, "dup");
		return (0);
	}
	return (1);
}

static int	app_redir(t_command *cmd, int s_stdin, int s_stdout, t_shell *shell)
{
	if (cmd->input_redir)
	{
		if (!setup_input_redirection(cmd->input_redir, shell))
		{
			restore_redirections(s_stdin, s_stdout);
			return (0);
		}
	}
	if (cmd->output_redir)
	{
		if (!setup_output_redirection(cmd->output_redir, shell))
		{
			restore_redirections(s_stdin, s_stdout);
			return (0);
		}
	}
	return (1);
}

static int	stp_redir(t_command *cmd, int *s_stdin, int *s_stdout, t_shell *sh)
{
	if (!save_standard_fds(s_stdin, s_stdout, sh))
		return (0);
	return (app_redir(cmd, *s_stdin, *s_stdout, sh));
}

/*
 * PIPELINE FUNCTIONS - COMPLETAMENTE CORREGIDO
 */
static int	safe_dup2(int oldfd, int newfd, const char *msg)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror(msg);
		return (0);
	}
	return (1);
}

static void	setup_pipe_input(int prev_pipe_read)
{
	if (prev_pipe_read != -1)
	{
		if (!safe_dup2(prev_pipe_read, STDIN_FILENO, "dup2 stdin"))
			exit(EXIT_FAILURE);
		close(prev_pipe_read);
	}
}

static void	setup_pipe_output(int *pipe_fd)
{
	close(pipe_fd[0]);
	if (!safe_dup2(pipe_fd[1], STDOUT_FILENO, "dup2 stdout"))
	{
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
}

static void	setup_input_file(t_redir_file *input_redir)
{
	int	fd;

	if (!input_redir || input_redir->is_heredoc)
		return ;
	fd = open(input_redir->expanded_path, O_RDONLY);
	if (fd == -1)
	{
		perror(input_redir->expanded_path);
		exit(EXIT_FAILURE);
	}
	if (!safe_dup2(fd, STDIN_FILENO, "dup2 input"))
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static int	get_output_flags(t_redir_file *output_redir)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (output_redir->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	return (flags);
}

static void	setup_output_file(t_redir_file *output_redir)
{
	int	fd;
	int	flags;

	if (!output_redir)
		return ;
	flags = get_output_flags(output_redir);
	fd = open(output_redir->expanded_path, flags, 0644);
	if (fd == -1)
	{
		perror(output_redir->expanded_path);
		exit(EXIT_FAILURE);
	}
	if (!safe_dup2(fd, STDOUT_FILENO, "dup2 output"))
	{
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
}

static void	setup_pipeline_redir(t_command *cmd, int p_pipe_read, int *pipe_fd)
{
	setup_pipe_input(p_pipe_read);
	if (cmd->next && cmd->logic == CMD_PIPE)
		setup_pipe_output(pipe_fd);
	setup_input_file(cmd->input_redir);
	setup_output_file(cmd->output_redir);
}

static int	execute_pipeline_command(t_command *cmd, t_shell *shell)
{
	char	**argv;
	int		exit_status;

	if (cmd->subshell)
		return (execute_command_tree(cmd->subshell, shell));
	argv = get_argv_from_args(cmd);
	if (!argv || !argv[0])
	{
		if (argv)
			free_argv(argv);
		return (EXIT_SUCCESS);
	}
	setup_signals_default();
	if (is_builtin(argv[0]))
		exit_status = execute_builtin(argv, shell);
	else
		exit_status = execute_external(argv, shell);
	free_argv(argv);
	return (exit_status);
}

/*
 * HELPER FUNCTIONS
 */
static int	has_pipeline(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->logic == CMD_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	has_logical_ops(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		if (current->logic == CMD_AND || current->logic == CMD_OR)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	count_pipeline_commands(t_command *cmd_list)
{
	t_command	*current;
	int			count;

	current = cmd_list;
	count = 0;
	while (current)
	{
		count++;
		if (current->logic != CMD_PIPE)
			break ;
		current = current->next;
	}
	return (count);
}

static int	prepare_argv(t_command *cmd, char ***argv)
{
	*argv = get_argv_from_args(cmd);
	if (!*argv || !(*argv)[0])
	{
		if (*argv)
			free_argv(*argv);
		return (0);
	}
	return (1);
}

/*
 * SINGLE COMMAND EXECUTION
 */
static int	execute_single_command(t_command *cmd, t_shell *shell)
{
	char	**argv;
	int		saved_stdin;
	int		saved_stdout;
	int		exit_status;

	if (!cmd || !shell)
		return (EXIT_FAILURE);
	if (cmd->subshell)
		return (execute_command_tree(cmd->subshell, shell));
	if (!prepare_argv(cmd, &argv))
		return (EXIT_SUCCESS);
	if (!stp_redir(cmd, &saved_stdin, &saved_stdout, shell))
	{
		free_argv(argv);
		return (EXIT_FAILURE);
	}
	if (is_builtin(argv[0]))
		exit_status = execute_builtin(argv, shell);
	else
		exit_status = execute_external(argv, shell);
	restore_redirections(saved_stdin, saved_stdout);
	free_argv(argv);
	return (set_exit_status(shell, exit_status));
}

static void	setup_heredoc_if_first(t_command *current, int is_first)
{
	if (!is_first)
		return ;
	if (!current->input_redir || !current->input_redir->is_heredoc)
		return ;
	if (current->input_redir->fd < 0)
		return ;
	if (!safe_dup2(current->input_redir->fd, STDIN_FILENO, "dup2 heredoc"))
		exit(EXIT_FAILURE);
	close(current->input_redir->fd);
	current->input_redir->fd = -1;
}

static void	exe_in_child(t_command *current, t_child_ctx *ctx, t_shell *shell)
{
	setup_signals_default();
	setup_heredoc_if_first(current, ctx->is_first);
	setup_pipeline_redir(current, ctx->prev_pipe_read, ctx->pipe_fd);
	exit(execute_pipeline_command(current, shell));
}

/*
 * PIPELINE PARENT PROCESS
 */
static void	handle_parent_fds(int *prev_pipe_read, t_command *current,
		int *pipe_fd)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (current->next && current->logic == CMD_PIPE)
	{
		close(pipe_fd[1]);
		*prev_pipe_read = pipe_fd[0];
	}
	else
		*prev_pipe_read = -1;
}

static int	get_process_exit_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit: 3\n", 8);
		return (128 + sig);
	}
	return (0);
}

static int	wait_pipeline(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	exit_status;

	exit_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			exit_status = get_process_exit_status(status);
		i++;
	}
	return (exit_status);
}

static void	close_pipe_fds(int prev_read, int *pipe_fd)
{
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (prev_read != -1)
		close(prev_read);
}

static void	cleanup_pipeline_error(t_pipe_ctx *ctx, int *pipe_fd)
{
	int	i;

	close_pipe_fds(ctx->prev_pipe_read, pipe_fd);
	i = 0;
	while (i < ctx->index)
	{
		if (ctx->pids[i] > 0)
			waitpid(ctx->pids[i], NULL, 0);
		i++;
	}
	free(ctx->pids);
}

static int	init_pipe_ctx(t_pipe_ctx *ctx, t_command *cmd_lst, t_shell *sh)
{
	ctx->cmd_count = count_pipeline_commands(cmd_lst);
	ctx->pids = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->pids)
		return (handle_error(sh, ERR_MALLOC, "pipeline execution"));
	ctx->prev_pipe_read = -1;
	ctx->index = 0;
	return (0);
}

static int	crt_pipe_if(t_command *crr, int *p_fd, t_pipe_ctx *ctx, t_shell *sh)
{
	if (crr->next && crr->logic == CMD_PIPE)
	{
		if (pipe(p_fd) == -1)
		{
			handle_system_error(sh, "pipe");
			cleanup_pipeline_error(ctx, NULL);
			return (0);
		}
	}
	return (1);
}

static int	fork_exe(t_command *curr, int *p_fd, t_pipe_ctx *ctx, t_shell *sh)
{
	int			has_pipe;
	t_child_ctx	child_ctx;

	has_pipe = (curr->next && curr->logic == CMD_PIPE);
	ctx->pids[ctx->index] = fork();
	if (ctx->pids[ctx->index] == -1)
	{
		handle_system_error(sh, "fork");
		if (has_pipe)
			cleanup_pipeline_error(ctx, p_fd);
		else
			cleanup_pipeline_error(ctx, NULL);
		return (0);
	}
	else if (ctx->pids[ctx->index] == 0)
	{
		child_ctx.prev_pipe_read = ctx->prev_pipe_read;
		child_ctx.pipe_fd = p_fd;
		child_ctx.is_first = (ctx->index == 0);
		exe_in_child(curr, &child_ctx, sh);
	}
	else
		handle_parent_fds(&ctx->prev_pipe_read, curr, p_fd);
	return (1);
}

static int	execute_pipeline(t_command *cmd_list, t_shell *shell)
{
	t_pipe_ctx		ctx;
	t_command		*current;
	int				pipe_fd[2];
	int				exit_status;

	if (init_pipe_ctx(&ctx, cmd_list, shell))
		return (EXIT_FAILURE);
	current = cmd_list;
	while (current && ctx.index < ctx.cmd_count)
	{
		if (!crt_pipe_if(current, pipe_fd, &ctx, shell))
			return (EXIT_FAILURE);
		if (!fork_exe(current, pipe_fd, &ctx, shell))
			return (EXIT_FAILURE);
		current = current->next;
		ctx.index++;
	}
	if (ctx.prev_pipe_read != -1)
		close(ctx.prev_pipe_read);
	exit_status = wait_pipeline(ctx.pids, ctx.cmd_count);
	free(ctx.pids);
	return (set_exit_status(shell, exit_status));
}

static t_command	*find_pipeline_end(t_command *start)
{
	t_command	*end;

	end = start;
	while (end->next && end->logic == CMD_PIPE)
		end = end->next;
	return (end);
}

static int	should_stop_execution(t_command *current, int exit_status)
{
	if (!current)
		return (1);
	if (current->logic == CMD_AND && exit_status != EXIT_SUCCESS)
		return (1);
	if (current->logic == CMD_OR && exit_status == EXIT_SUCCESS)
		return (1);
	return (0);
}

static int	execute_logical_sequence(t_command *cmd_list, t_shell *shell)
{
	t_command	*current;
	t_command	*pipeline_end;
	int			exit_status;

	current = cmd_list;
	exit_status = EXIT_SUCCESS;
	while (current)
	{
		if (current->logic == CMD_PIPE)
		{
			pipeline_end = find_pipeline_end(current);
			exit_status = execute_pipeline(current, shell);
			current = pipeline_end->next;
		}
		else
		{
			exit_status = execute_single_command(current, shell);
			current = current->next;
		}
		if (should_stop_execution(current, exit_status))
			break ;
	}
	return (exit_status);
}

/*
 * MAIN EXECUTION DISPATCHER
 */
int	execute_command_tree(t_command *cmd_tree, t_shell *shell)
{
	if (!cmd_tree || !shell)
		return (EXIT_FAILURE);
	if (has_pipeline(cmd_tree))
		return (execute_pipeline(cmd_tree, shell));
	else if (has_logical_ops(cmd_tree))
		return (execute_logical_sequence(cmd_tree, shell));
	else
		return (execute_single_command(cmd_tree, shell));
}
