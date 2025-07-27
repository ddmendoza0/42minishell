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
	EXPAND
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

/*AST Builder*/
typedef struct s_command {
    char **argv;
    char *input_file;
    char *output_file;
    int append_output;
    int heredoc;
    struct s_command *next;
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


/****************************************************************/
/*			END DECLARATIONS	 		*/
/****************************************************************/
#endif
