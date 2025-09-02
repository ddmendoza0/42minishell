
#include "minishell.h"
# include <sys/wait.h>

/*
 * UTILITY FUNCTIONS
 */
void	free_argv(char** argv)
{
	int	i;

	if (!argv)
		return;
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
static int	is_builtin(char* cmd)
{
	
	if (!cmd)
		return (0);
	//if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
	//	return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (1);
	//if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
	//	return (1);
	//if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
	//	return (1);
	//if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
	//	return (1);
	//if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
	//	return (1);
	return (0);
}

/*
 * BUILTIN COMMAND DISPATCHER
 */
static int	execute_builtin(char** argv, t_shell* shell)
{
	char* cmd;

	
	if (!argv || !argv[0] || !shell)
		return (EXIT_FAILURE);
	cmd = argv[0];
	/* uncoment this
	if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (builtin_echo(argv));
	else if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (builtin_cd(argv, shell));
	else if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (builtin_pwd());
	else if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (builtin_export(argv, shell));
	else if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (builtin_unset(argv, shell));
	else if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (builtin_env(shell));
	else if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (builtin_exit(argv, shell));
	return (EXIT_FAILURE);
	erase all bellow*/

	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (builtin_cd(argv, shell));
	else if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (builtin_pwd(shell));
	ft_putstr_fd("minishell: builtin not implemented xd\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}

/*
 * HEREDOC IMPLEMENTATION
 */
static int	setup_heredoc(char* delimiter, t_shell* shell)
{
	int		pipe_fd[2];
	char* line;
	pid_t	pid;

	(void)shell;
	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	else if (pid == 0)
	{
		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			{
				if (line)
					free(line);
				break;
			}
			write(pipe_fd[1], line, ft_strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}
		close(pipe_fd[1]);
		exit(0);
	}
	else
	{
		close(pipe_fd[1]);
		waitpid(pid, NULL, 0);
		return (pipe_fd[0]);
	}
}

/*
 * PATH RESOLUTION HELPERS
 */
static char* create_full_path(char* dir, char* cmd)
{
	char* temp;
	char* result;

	if (!dir || !cmd)
		return (NULL);
	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, cmd);
	free(temp);
	return (result);
}

static char* find_executable(char* cmd, t_shell* shell)
{
	char* path_env;
	char** path_dirs;
	char* full_path;
	int		i;

	if (!cmd || !shell)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value(shell, "PATH");
	if (!path_env)
		return (NULL);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		full_path = create_full_path(path_dirs[i], cmd);
		if (full_path && access(full_path, F_OK | X_OK) == 0)
		{
			free_argv(path_dirs);
			return (full_path);
		}
		if (full_path)
			free(full_path);
		i++;
	}
	free_argv(path_dirs);
	return (NULL);
}

/*
 * EXTERNAL COMMAND EXECUTION
 */
static int	execute_external(char** argv, t_shell* shell)
{
	char* executable_path;
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
	{
		execve(executable_path, argv, shell->env);
		handle_system_error(shell, argv[0]);
		exit(EXIT_CANNOT_EXECUTE);
	}
	else
	{
		waitpid(pid, &status, 0);
		free(executable_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (EXIT_SIGNAL_BASE + WTERMSIG(status));
	}
	return (EXIT_FAILURE);
}

/*
 * REDIRECTION SETUP FUNCTIONS
 */
static int	setup_input_redirection(t_redir_file* input_redir, t_shell* shell)
{
	int	fd;

	if (input_redir->is_heredoc)
		fd = setup_heredoc(input_redir->expanded_path, shell);
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
	return (1);
}

static int	setup_output_redirection(t_redir_file* output_redir, t_shell* shell)
{
	int	fd;
	int	flags;

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
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

static int	setup_redirections(t_command* cmd, int* saved_stdin, int* saved_stdout,
	t_shell* shell)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		handle_system_error(shell, "dup");
		return (0);
	}
	if (cmd->input_redir)
	{
		if (!setup_input_redirection(cmd->input_redir, shell))
			return (0);
	}
	if (cmd->output_redir)
	{
		if (!setup_output_redirection(cmd->output_redir, shell))
			return (0);
	}
	return (1);
}

/*
 * PIPELINE FUNCTIONS
 */
static void	setup_pipeline_redirections(t_command* cmd, int prev_pipe_read,
	int* pipe_fd)
{
	if (prev_pipe_read != -1)
	{
		dup2(prev_pipe_read, STDIN_FILENO);
		close(prev_pipe_read);
	}
	if (cmd->next && cmd->logic == CMD_PIPE)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (cmd->input_redir && !cmd->input_redir->is_heredoc)
	{
		int	fd = open(cmd->input_redir->expanded_path, O_RDONLY);
		if (fd != -1)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	if (cmd->output_redir)
	{
		int	flags = O_WRONLY | O_CREAT;
		int	fd;

		if (cmd->output_redir->append_mode)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(cmd->output_redir->expanded_path, flags, 0644);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

static int	execute_pipeline_command(t_command* cmd, t_shell* shell)
{
	char** argv;
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
static int	has_pipeline(t_command* cmd_list)
{
	t_command* current;

	current = cmd_list;
	while (current)
	{
		if (current->logic == CMD_PIPE)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	has_logical_ops(t_command* cmd_list)
{
	t_command* current;

	current = cmd_list;
	while (current)
	{
		if (current->logic == CMD_AND || current->logic == CMD_OR)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	count_pipeline_commands(t_command* cmd_list)
{
	t_command* current;
	int			count;

	current = cmd_list;
	count = 0;
	while (current)
	{
		count++;
		if (current->logic != CMD_PIPE)
			break;
		current = current->next;
	}
	return (count);
}

/*
 * SINGLE COMMAND EXECUTION
 */
static int	execute_single_command(t_command* cmd, t_shell* shell)
{
	char** argv;
	int		saved_stdin;
	int		saved_stdout;
	int		exit_status;

	if (!cmd || !shell)
		return (EXIT_FAILURE);
	if (cmd->subshell)
		return (execute_command_tree(cmd->subshell, shell));
	argv = get_argv_from_args(cmd);
	if (!argv || !argv[0])
	{
		if (argv)
			free_argv(argv);
		return (EXIT_SUCCESS);
	}
	saved_stdin = -1;
	saved_stdout = -1;
	if (!setup_redirections(cmd, &saved_stdin, &saved_stdout, shell))
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

/*
 * PIPELINE EXECUTION
 */
static int	execute_pipeline(t_command* cmd_list, t_shell* shell)
{
	t_command* current;
	int			pipe_fd[2];
	int			prev_pipe_read;
	pid_t* pids;
	int			cmd_count;
	int			i;
	int			status;
	int			exit_status;

	cmd_count = count_pipeline_commands(cmd_list);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (handle_error(shell, ERR_MALLOC, "pipeline execution"));
	current = cmd_list;
	prev_pipe_read = -1;
	i = 0;
	while (current)
	{
		if (current->next && current->logic == CMD_PIPE)
		{
			if (pipe(pipe_fd) == -1)
			{
				handle_system_error(shell, "pipe");
				free(pids);
				return (EXIT_FAILURE);
			}
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			handle_system_error(shell, "fork");
			free(pids);
			return (EXIT_FAILURE);
		}
		else if (pids[i] == 0)
		{
			setup_pipeline_redirections(current, prev_pipe_read, pipe_fd);
			exit(execute_pipeline_command(current, shell));
		}
		else
		{
			if (prev_pipe_read != -1)
				close(prev_pipe_read);
			if (current->next && current->logic == CMD_PIPE)
			{
				close(pipe_fd[1]);
				prev_pipe_read = pipe_fd[0];
			}
		}
		current = current->next;
		i++;
	}
	exit_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			exit_status = WEXITSTATUS(status);
		i++;
	}
	free(pids);
	return (set_exit_status(shell, exit_status));
}

/*
 * LOGICAL SEQUENCE EXECUTION
 */
static int	execute_logical_sequence(t_command* cmd_list, t_shell* shell)
{
	t_command* current;
	t_command* pipeline_start;
	t_command* pipeline_end;
	int			exit_status;

	current = cmd_list;
	exit_status = EXIT_SUCCESS;
	while (current)
	{
		if (current->logic == CMD_PIPE)
		{
			pipeline_start = current;
			pipeline_end = current;
			while (pipeline_end->next && pipeline_end->logic == CMD_PIPE)
				pipeline_end = pipeline_end->next;
			exit_status = execute_pipeline(pipeline_start, shell);
			current = pipeline_end->next;
		}
		else
			exit_status = execute_single_command(current, shell);
		if (current && current->logic == CMD_AND && exit_status != EXIT_SUCCESS)
			break ;
		else if (current && current->logic == CMD_OR \
			&& exit_status == EXIT_SUCCESS)
			break ;
		if (current)
			current = current->next;
	}
	return (exit_status);
}

/*
 * MAIN EXECUTION DISPATCHER
 */
int	execute_command_tree(t_command* cmd_tree, t_shell* shell)
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