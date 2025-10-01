/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:45 by dmendoza          #+#    #+#             */
/*   Updated: 2025/09/18 12:20:50 by dmaya-vi         ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>


# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libft.h"
# include <fcntl.h>
//for the readline function and functioning history
# include <readline/readline.h>
# include <readline/history.h>
//error manager
# include "error_manager.h"
//executor
#include <sys/wait.h>
//signals
#include <signal.h>
extern volatile sig_atomic_t g_signal_received;

/****************************************************************/
/*			END LIBRARIES				*/
/****************************************************************/

/****************************************************************/
/*			DEFINES					*/
/****************************************************************/
# define WHITESPACES " \t\n"
# define MAX_HISTORY 1000
# define HISTORY_FILE ".minishell_history"
#define TMP_IN_FILE  "/tmp/minishell_tmp_in"
#define TMP_OUT_FILE "/tmp/minishell_tmp_out"
#define TMP_HEREDOC_FILE "/tmp/minishell_tmp_heredoc"

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
	SEMICOLON,
	INVALID
}t_token_type;

typedef enum e_quote_type {
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
} t_quote_type;

typedef struct s_token_segment {
	char* content;
	t_quote_type quote_type;
	struct s_token_segment* next;
} t_token_segment;

typedef struct s_token
{
	char		*value;
	t_token_segment *segments;
	t_token_type	type;
	int		fd;
	struct	s_token	*next;
}t_token;

typedef struct s_segment_data
{
	size_t	j;
	size_t	word_len;
	size_t	segment_start;
}	t_segment_data;
/*END TOKEN*/

/*SEHLL STRUCTURE*/
typedef struct s_shell
{
	char** env;
	char* cwd;
	int     last_exit_status;
	int     stdin_backup;
	int     stdout_backup;
} t_shell;
/*SEHLL STRUCTURE*/

/*AST Builder - UPDATED TO PRESERVE SEGMENTS*/
typedef enum e_cmd_logic
{
	CMD_NONE,
	CMD_AND,
	CMD_OR,
	CMD_PIPE
}   t_cmd_logic;

// Nueva estructura para argumentos que preserva segmentos
typedef struct s_arg_token {
	t_token* original_token;     // Token og
	char* expanded_value;        // Value expandido 
	struct s_arg_token* next;
} t_arg_token;

// Nueva estructura para archivos de redirecci�n
typedef struct s_redir_file {
	t_token* original_token;     // Token og
	char* expanded_path;         // Ruta expandida
	int append_mode;             // 1 para >>, 0 para >
	int is_heredoc;              // 1 para <<, 0 para <
	int fd;                      // Descriptor de archivo
} t_redir_file;

typedef struct s_command {
	t_arg_token* args;           // Lista de args con segmentos
	t_redir_file* input_redir;   // Redirecci�n de entrada
	t_redir_file* output_redir;  // Redirecci�n de salida
	t_cmd_logic logic;
	struct s_command* subshell;
	struct s_command* next;
} t_command;
/*AST Builder*/

/****************************************************************/
/*			END DEFINES				*/
/****************************************************************/


/****************************************************************/
/*			DECLARATIONS		 		*/
/****************************************************************/

void	write_to_history_file(char *input, int history_fd);
int		initialize_history(void);
void	cmd_history(void);

//shell
int		init_shell(t_shell *shell, char **envp);
void	cleanup_shell(t_shell *shell);

//lexer
int				lexer(char *input, t_token **token_lst, t_shell *shell);
t_token			*create_token(void *value, t_token_type type);
t_token			*last_token(t_token *token);
void			addback_token(t_token **lst, t_token *new);
void			free_token_lst(t_token *lst);
void			free_segments(t_token_segment *segments);
int				extract_word(size_t *i, const char *input, t_token **lst, t_shell *shell);
int				extract_oprtr(size_t *i, const char *input, t_token **lst, t_shell *shell);
int				is_single_char_operator(char c);
t_token_segment	*create_segment(char *content, t_quote_type quote_type);
void			add_segment(t_token_segment **sgmnts, t_token_segment *n_sgmnt);
int				find_closing_quote(const char *input, char quote_char, size_t start);
char			*extract_content(const char *input, size_t start, size_t end);
int				create_default_segment(t_token *token, char *word);
int				process_word_segments(t_token *token, char *word, t_shell* shell);
int				process_segments(t_token *token, char *word, t_shell *shell, t_token **lst);
int				handle_q_sctn(size_t *i, const char *input, t_shell *shell, t_token **lst);

// Token and argument management
t_arg_token		*create_arg_token(t_token *original);
void			add_arg_token(t_arg_token **list, t_arg_token *new_arg);
void			free_arg_tokens(t_arg_token *args);

// Redirection management
t_redir_file	*create_redir_file(t_token *original, int append_mode, int is_heredoc);
void			free_redir_file(t_redir_file *redir);

// Parser functions
t_command		*cmd_builder(t_token **tkn_list, t_shell *shell);
int				create_cmd(t_command **cmd);
void			free_cmd_list(t_command *cmd);
int				add_token_to_args(t_command *cmd, t_token *token);
int				add_token_redir_in(t_command *cmd, t_token **token, t_shell *shell);
int				add_token_redir_out(t_command *cmd, t_token **token, t_shell *shell);
int				handle_lparen(t_command *cmd, t_token **current, t_shell *shell);
t_token			*extract_subshell_tokens(t_token **current);
int				validate_command_redirections(t_command *cmd);
int				handle_word_token(t_command *cmd, t_token **current);
int				handle_logic_token(t_command *cmd, t_token *current, t_command **new_cmd);
int				handle_token_error(t_token *current, t_shell *shell);

// Expansion and review functions
int				lexical_review(t_command *cmd_list, t_shell *shell);
char			*expand_from_segments(t_token_segment *segments, t_shell *shell);
char			*expand_variables_in_string(char *str, t_shell *shell);
char			*expand_token(t_token *token, t_shell *shell);
char			*process_segment(t_token_segment *segment, t_shell *shell);

// Environment utilities
char			*get_env_value(t_shell *shell, char *var_name);
char			*get_special_var(t_shell *shell, char *var_name);
char			*expand_variable(t_shell *shell, char *var_name);

// Execution helper functions (implemented in token_management.c)
char			**get_argv_from_args(t_command *cmd);
char			*get_input_file(t_command *cmd);
char			*get_output_file(t_command *cmd);
int				is_append_mode(t_command *cmd);
int				is_heredoc_mode(t_command *cmd);

 // Executor
int				execute_command_tree(t_command *cmd_tree, t_shell *shell);

//Builtins
int				set_env_var(t_shell *shell, char *var_assignment);
int				builtin_cd(char **argv, t_shell *shell);
int				builtin_pwd(char **argv, t_shell *shell);
int				builtin_echo(char **argv, t_shell *shell);
int				builtin_export(char **argv, t_shell *shell);
int				builtin_unset(char **argv, t_shell *shell);
int				builtin_env(char **argv, t_shell *shell);
int				builtin_exit(char **argv, t_shell *shell);

// History commands
void	write_to_history_file(char *input, int history_fd);
int	initialize_history(void);
void     cmd_history(void);
//mendo express repairs
void trim_history_file(void);

//signals
void	setup_signals_interactive(void);
void	setup_signals_execution(void);
void	setup_signals_default(void);
void	handle_interactive_signals(t_shell *shell);
int		get_signal_exit_status(int sig);

// Utility function
void			free_argv(char **argv);

/****************************************************************/
/*			END DECLARATIONS	 		*/
/****************************************************************/
#endif
