/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:45 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/26 13:04:10 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/****************************************************************/
/*			LIBRARIES		 		*/
/****************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include "libft.h"
# include "error_manager.h"
# include <sys/types.h>
# include <sys/stat.h>
#include <limits.h>

//for the readline function and functioning history
# include <readline/readline.h>
# include <readline/history.h>
//error manager
# include "error_manager.h"
//executor
# include <sys/wait.h>
//signals
# include <signal.h>

extern volatile sig_atomic_t	g_signal_received;

/****************************************************************/
/*			END LIBRARIES				*/
/****************************************************************/

/****************************************************************/
/*			DEFINES					*/
/****************************************************************/
# define WHITESPACES " \t\n"
# define MAX_HISTORY 1000
# define HISTORY_FILE ".minishell_history"
# define TMP_IN_FILE  "/tmp/minishell_tmp_in"
# define TMP_OUT_FILE "/tmp/minishell_tmp_out"
# define TMP_HEREDOC_FILE "/tmp/minishell_tmp_heredoc"

/*BEGIN TOKEN*/
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND_OUT,
	HEREDOC,
	AND,
	OR,
	LPAREN,
	RPAREN,
	INVALID
}	t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_token_segment
{
	char					*content;
	t_quote_type			quote_type;
	struct s_token_segment	*next;
}	t_token_segment;

typedef struct s_token
{
	char			*value;
	t_token_segment	*segments;
	t_token_type	type;
	int				fd;
	struct s_token	*next;
}	t_token;

typedef struct s_segment_data
{
	size_t	j;
	size_t	word_len;
	size_t	segment_start;
}	t_segment_data;

typedef struct s_expand_data
{
	char	*str;
	char	*result;
	size_t	i;
	size_t	j;
	size_t	result_size;
	t_shell	*shell;
}	t_expand_data;
/*END TOKEN*/

/*SEHLL STRUCTURE*/
typedef struct s_shell
{
	char	**env;
	char	*cwd;
	int		last_exit_status;
	int		stdin_backup;
	int		stdout_backup;
}	t_shell;
/*SEHLL STRUCTURE*/

/*AST Builder - UPDATED TO PRESERVE SEGMENTS*/
typedef enum e_cmd_logic
{
	CMD_NONE,
	CMD_AND,
	CMD_OR,
	CMD_PIPE
}	t_cmd_logic;

// Nueva estructura para argumentos que preserva segmentos
typedef struct s_arg_token
{
	t_token				*original_token;
	char				*expanded_value;
	struct s_arg_token	*next;
}	t_arg_token;

// Nueva estructura para archivos de redirecci�n
typedef struct s_redir_file
{
	t_token	*original_token;
	char	*expanded_path;
	int		append_mode;
	int		is_heredoc;
	int		fd;
	int		order;
}	t_redir_file;

typedef struct s_redir_list
{
	t_redir_file			*redir;
	struct s_redir_list		*next;
}	t_redir_list;

typedef struct s_command
{
	t_arg_token			*args;
	t_redir_list		*input_redirs;
	t_redir_list		*output_redirs;
	t_cmd_logic			logic;
	struct s_command	*subshell;
	struct s_command	*next;
}	t_command;
/*AST Builder*/

/*PIPE*/
typedef struct s_pipe_ctx
{
	pid_t	*pids;
	int		cmd_count;
	int		prev_pipe_read;
	int		index;
}	t_pipe_ctx;

typedef struct s_child_ctx
{
	int	prev_pipe_read;
	int	*pipe_fd;
	int	is_first;
}	t_child_ctx;
/*PIPE*/

/****************************************************************/
/*			END DEFINES				*/
/****************************************************************/
/****************************************************************/
/*			DECLARATIONS		 		*/
/****************************************************************/
//main utils
char			*trim_input(const char *input);
void			cleanup_resources(t_command *cmd_tree,
					t_token *token_lst, char *clean_input);
void			execute_and_handle_signals(t_command *cmd_tree,
					t_shell *shell, char *input);
//history
void			write_to_history_file(char *input, int history_fd);
int				initialize_history(void);
void			cmd_history(void);

//shell
int				init_shell(t_shell *shell, char **envp);
void			cleanup_shell(t_shell *shell);

//lexer
int				lexer(char *input, t_token **token_lst, t_shell *shell);
t_token			*create_token(void *value, t_token_type type);
t_token			*last_token(t_token *token);
void			addback_token(t_token **lst, t_token *new);
void			free_token_lst(t_token *lst);
void			free_segments(t_token_segment *segments);
int				extract_word(size_t *i, const char *input,
					t_token **lst, t_shell *shell);
int				extract_oprtr(size_t *i, const char *input,
					t_token **lst, t_shell *shell);
int				is_single_char_operator(char c);
t_token_segment	*create_segment(char *content, t_quote_type quote_type);
void			add_segment(t_token_segment **sgmnts, t_token_segment *n_sgmnt);
int				find_closing_quote(const char *input,
					char quote_char, size_t start);
char			*extract_content(const char *input, size_t start, size_t end);
int				create_default_segment(t_token *token, char *word);
int				process_word_segments(t_token *token,
					char *word, t_shell *shell);
int				process_segments(t_token *token, char *word,
					t_shell *shell, t_token **lst);
int				handle_q_sctn(size_t *i, const char *input,
					t_shell *shell, t_token **lst);

//Token handler
char			**get_argv_from_args(t_command *cmd);
t_arg_token		*create_arg_token(t_token *original);
void			add_arg_token(t_arg_token **list, t_arg_token *new_arg);
void			free_arg_tokens(t_arg_token *args);
int				add_token_to_args(t_command *cmd, t_token *token);
int				add_token_redir_in(t_command *cmd,
					t_token **token, t_shell *shell);
int				add_token_redir_out(t_command *cmd,
					t_token **token, t_shell *shell);
t_redir_file	*create_redir_file(t_token *org, int append_mode, int is_hdoc);
void			free_redir_file(t_redir_file *redir);
char			*expand_variables_in_string(char *str, t_shell *shell);
char			*get_var_name(char *str, size_t *i);
char			*expand_from_segments(t_token_segment *segments,
					t_shell *shell);
char			*get_input_file(t_command *cmd);
char			*get_output_file(t_command *cmd);
int				is_append_mode(t_command *cmd);
int				is_heredoc_mode(t_command *cmd);

// Parser functions
t_command		*cmd_builder(t_token **tkn_list, t_shell *shell);
int				create_cmd(t_command **cmd);
void			free_cmd_list(t_command *cmd);
int				handle_lparen(t_command *cmd, t_token **current,
					t_shell *shell);
t_token			*extract_subshell_tokens(t_token **current);
int				validate_command_redirections(t_command *cmd, t_shell *shell);
int				handle_word_token(t_command *cmd, t_token **current);
int				handle_logic_token(t_command *cmd,
					t_token *current, t_command **new_cmd);
int				handle_token_error(t_token *current, t_shell *shell);
int				expand_command_args(t_command *cmd, t_shell *shell);
int				expand_redir_list(t_redir_list *list, t_shell *shell);

// Expansion and review functions
int				lexical_review(t_command *cmd_list, t_shell *shell);
char			*expand_token(t_token *token, t_shell *shell);
char			*process_segment(t_token_segment *segment, t_shell *shell);

// Environment utilities
char			*get_env_value(t_shell *shell, char *var_name);
char			*get_special_var(t_shell *shell, char *var_name);
char			*expand_variable(t_shell *shell, char *var_name);

// Executor
void			free_argv(char **argv);
int				has_pipeline(t_command *cmd_list);
int				has_logical_ops(t_command *cmd_list);
int				count_pipeline_commands(t_command *cmd_list);
int				prepare_argv(t_command *cmd, char ***argv);
int				is_builtin(char *cmd);
int				execute_builtin(char **argv, t_command *cmd, t_shell *shell);
char			*find_executable(char *cmd, t_shell *shell);
int				execute_external(char **argv, t_shell *shell);
void			restore_redirections(int saved_stdin, int saved_stdout);
int				stp_redir(t_command *cmd, int *s_stdin,
					int *s_stdout, t_shell *sh);
int				save_standard_fds(int *s_stdin, int *s_stdout, t_shell *shell);
int				app_redir(t_command *cmd, int s_stdin,
					int s_stdout, t_shell *shell);
int				safe_dup2(int oldfd, int newfd, const char *msg);
void			setup_pipe_input(int prev_pipe_read);
void			setup_pipe_output(int *pipe_fd);
void			setup_input_file(t_redir_file *input_redir);
int				get_output_flags(t_redir_file *output_redir);
void			setup_pipeline_redir(t_command *cmd,
					int p_pipe_read, int *pipe_fd);
void			exe_in_child(t_command *current,
					t_child_ctx *ctx, t_shell *shell);
void			handle_parent_fds(int *prev_pipe_read,
					t_command *current, int *pipe_fd);
int				wait_pipeline(pid_t *pids, int cmd_count);
void			cleanup_pipeline_error(t_pipe_ctx *ctx, int *pipe_fd);
int				init_pipe_ctx(t_pipe_ctx *ctx, t_command *cmd_lst, t_shell *sh);
int				crt_pipe_if(t_command *crr, int *p_fd,
					t_pipe_ctx *ctx, t_shell *sh);
int				fork_exe(t_command *curr, int *p_fd,
					t_pipe_ctx *ctx, t_shell *sh);
int				execute_pipeline(t_command *cmd_list, t_shell *shell);
int				execute_logical_sequence(t_command *cmd_list, t_shell *shell);
int				execute_single_command(t_command *cmd, t_shell *shell);
int				execute_command_tree(t_command *cmd_tree, t_shell *shell);
char			**remove_empty_args(char **argv);

//Builtins
int				set_env_var(t_shell *shell, char *var_assignment);
int				builtin_cd(char **argv, t_shell *shell);
int				builtin_pwd(char **argv, t_shell *shell);
int				builtin_echo(char **argv, t_command *cmd, t_shell *shell);
int				builtin_export(char **argv, t_shell *shell);
int				builtin_unset(char **argv, t_shell *shell);
int				builtin_env(char **argv, t_shell *shell);
int				builtin_exit(char **argv, t_shell *shell);

// History commands
void			write_to_history_file(char *input, int history_fd);
int				initialize_history(void);
void			cmd_history(void);
//mendo express repairs
void			trim_history_file(void);

//signals
void			setup_signals_interactive(void);
void			setup_signals_execution(void);
void			setup_signals_default(void);
void			handle_interactive_signals(t_shell *shell);
int				get_signal_exit_status(int sig);

// Redir list functions
t_redir_list	*create_redir_list_node(t_redir_file *redir);
void			add_redir_to_list(t_redir_list **list, t_redir_file *redir);
void			free_redir_list(t_redir_list *list);
t_redir_file	*get_last_redir(t_redir_list *list);
int				handle_heredoc(t_redir_file *redir);
int				process_heredocs(t_command *cmd, t_shell *shell);
int				validate_single_input(t_redir_file *redir, t_shell *shell);
int				validate_single_output(t_redir_file *redir, t_shell *shell);
t_redir_file	*check_input_redirs(t_command *cmd,
					int current_order, int *min_order);
t_redir_file	*get_next_redir_by_order(t_command *cmd, int current_order);
void			close_previous_redirs(t_command *cmd, int up_to_order);
int				check_if_input(t_redir_file *redir, t_command *cmd);
int				validate_command_redirections(t_command *cmd, t_shell *shell);

// Updated validation signatures
int				validate_input_redirection(t_redir_list *input_redirs,
					t_shell *shell);
int				validate_output_redirection(t_redir_list *output_redirs,
					t_shell *shell);
// Utility function
void			free_argv(char **argv);
/****************************************************************/
/*			END DECLARATIONS	 		*/
/****************************************************************/
#endif
