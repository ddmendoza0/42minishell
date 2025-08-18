/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:45 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/15 17:56:19 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
/****************************************************************/
/*			DEFINES					*/
/****************************************************************/
# define WHITESPACES " \t\n"

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

// Nueva estructura para archivos de redirección
typedef struct s_redir_file {
	t_token* original_token;     // Token og
	char* expanded_path;         // Ruta expandida
	int append_mode;             // 1 para >>, 0 para >
	int is_heredoc;              // 1 para <<, 0 para <
} t_redir_file;

typedef struct s_command {
	t_arg_token* args;           // Lista de args con segmentos
	t_redir_file* input_redir;   // Redirección de entrada
	t_redir_file* output_redir;  // Redirección de salida
	t_cmd_logic logic;
	struct s_command* subshell;
	struct s_command* next;
} t_command;
/*AST Builder*/

/****************************************************************/
/*			END DEFINES				*/
/****************************************************************/
/****************************************************************/
/*			LIBRARIES		 		*/
/****************************************************************/

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
/****************************************************************/
/*			END LIBRARIES				*/
/****************************************************************/

# define HISTORY_FILE ".minishell_history"

/****************************************************************/
/*			DECLARATIONS		 		*/
/****************************************************************/

void	write_to_history_file(char *input, int history_fd);
int		initialize_history(void);
void	cmd_history(void);

//shell
int		init_shell(t_shell * shell, char** envp);
void	cleanup_shell(t_shell* shell);

//lexer
int		lexer(char* input, t_token** token_lst, t_shell* shell);
t_token	*create_token(void *value, t_token_type type);
t_token	*last_token(t_token *token);
void	addback_token(t_token **lst, t_token *new);
void	free_token_lst(t_token *lst);
void	free_segments(t_token_segment* segments);

// Token and argument management
t_arg_token* create_arg_token(t_token* original);
void add_arg_token(t_arg_token** list, t_arg_token* new_arg);
void free_arg_tokens(t_arg_token* args);

// Redirection management
t_redir_file* create_redir_file(t_token* original, int append_mode, int is_heredoc);
void free_redir_file(t_redir_file* redir);

// Parser functions
t_command* cmd_builder(t_token** tkn_list);
void free_cmd_list(t_command* cmd);
int add_token_to_args(t_command* cmd, t_token* token);
int add_token_redir_in(t_command* cmd, t_token** token);
int add_token_redir_out(t_command* cmd, t_token** token);
int handle_lparen(t_command* cmd, t_token** current);
t_token* extract_subshell_tokens(t_token** current);

// Expansion and review functions
int lexical_review(t_command* cmd_list, t_shell* shell);
char* expand_from_segments(t_token_segment* segments, t_shell* shell);
char* expand_variables_in_string(char* str, t_shell* shell);

// Environment utilities
char* get_env_value(t_shell* shell, char* var_name);
char* get_special_var(t_shell* shell, char* var_name);
char* expand_variable(t_shell* shell, char* var_name);

// Utility functions
void print_command_tree(t_command* cmd, int depth);
void print_expansion_debug(t_command* cmd);

// Execution helper functions (implemented in token_management.c)
char** get_argv_from_args(t_command* cmd);
char* get_input_file(t_command* cmd);
char* get_output_file(t_command* cmd);
int is_append_mode(t_command* cmd);
int is_heredoc_mode(t_command* cmd);

/****************************************************************/
/*			END DECLARATIONS	 		*/
/****************************************************************/
#endif
