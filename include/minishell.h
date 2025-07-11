/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:45 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/11 19:30:03 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
/****************************************************************/
/*			DEFINES					*/
/****************************************************************/
# define WHITESPACES " \t\n"

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND_OUT,
	HEREDOC
}t_token_type;

typedef struct s_token
{
	char		*value;
	t_token_type	type;
	struct	s_token	*next;
}t_toke;

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


void	write_to_history_file(char *input, int history_fd);
int	initialize_history(void);
void	cmd_history(void);
#endif
