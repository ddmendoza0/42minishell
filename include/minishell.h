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

/*BEGIN ERROR*/
typedef enum e_error_code
{
	// Success
	SUCCESS = 0,

	// Memory errors
	ERR_MALLOC = 1,
	ERR_OUT_OF_MEMORY = 2,

	// Lexer errors
	ERR_UNCLOSED_QUOTE = 10,
	ERR_INVALID_TOKEN = 11,
	ERR_LEXER_FAILURE = 12,

	// Parser errors  
	ERR_SYNTAX_ERROR = 20,
	ERR_UNEXPECTED_TOKEN = 21,
	ERR_MISSING_COMMAND = 22,
	ERR_INVALID_PIPE = 23,
	ERR_INVALID_REDIRECT = 24,
	ERR_UNMATCHED_PARENTHESES = 25,

	// Execution errors
	ERR_COMMAND_NOT_FOUND = 30,
	ERR_PERMISSION_DENIED = 31,
	ERR_FILE_NOT_FOUND = 32,
	ERR_IS_DIRECTORY = 33,
	ERR_NO_SUCH_FILE = 34,
	ERR_PIPE_FAILED = 35,
	ERR_FORK_FAILED = 36,
	ERR_EXEC_FAILED = 37,

	// Redirection errors
	ERR_REDIRECT_FAILED = 40,
	ERR_AMBIGUOUS_REDIRECT = 41,
	ERR_BAD_FD = 42,

	// Built-in errors
	ERR_BUILTIN_FAILED = 50,
	ERR_CD_FAILED = 51,
	ERR_EXPORT_FAILED = 52,
	ERR_UNSET_FAILED = 53,
	ERR_EXIT_INVALID_ARG = 54,

	// Environment errors
	ERR_ENV_VAR_NOT_FOUND = 60,
	ERR_ENV_EXPANSION_FAILED = 61,

	// General errors
	ERR_GENERIC = 99
} t_error_code;

// Error context structure
typedef struct s_error_context
{
	t_error_code	code;
	char* message;
	char* context;	// Additional context (e.g., token value, filename)
	int				line;		// For future use if you add line numbers
	int				column;		// For future use if you add column tracking
} t_error_context;

// Global error state (you might want to include this in a main shell structure)
typedef struct s_shell_state
{
	t_error_context	last_error;
	int				exit_code;
	// Add other global state here as needed
} t_shell_state;
/*END ERROR*/

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
	EXPAND
}t_token_type;

typedef enum e_quote_type {
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
} t_quote_type;

typedef struct s_token
{
	char		*value;
	t_quote_type quote_type;
	t_token_type	type;
	int		fd;
	struct	s_token	*next;
}t_token;
/*END TOKEN*/


typedef struct s_command {
    char **argv;
    char *input_file;
    char *output_file;
    int append_output;
    int heredoc;
    struct s_command *next;
} t_command;


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
/****************************************************************/
/*			END LIBRARIES				*/
/****************************************************************/

# define HISTORY_FILE ".minishell_history"

/****************************************************************/
/*			DECLARATIONS		 		*/
/****************************************************************/

void	write_to_history_file(char *input, int history_fd);
int	initialize_history(void);
void	cmd_history(void);

//error
void		set_error(t_shell_state* shell, t_error_code code, char* context);
void		clear_error(t_shell_state* shell);
void		print_error(t_shell_state* shell);
char* get_error_message(t_error_code code);
int			has_error(t_shell_state* shell);
int			get_exit_code(t_error_code code);
void cleanup_shell_state(t_shell_state* shell);
void init_shell_state(t_shell_state* shell);

//tokenizador
int lexer(char* input, t_token** token_lst, t_shell_state* shell);
t_token	*create_token(void *value, t_token_type type);
t_token	*last_token(t_token *token);
void	addback_token(t_token **lst, t_token *new);
void	free_token_lst(t_token *lst);

/****************************************************************/
/*			END DECLARATIONS	 		*/
/****************************************************************/
#endif
